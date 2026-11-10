#include "kit.h"

static uint8_t types[256]; // is this tok_t.type used for var declaration (in: <type> ident)?
__attribute__((constructor))
static void start() {
    uint8_t var_types[] = {
        type, kw_func, kw_let,
        kw_var, type_ident
    };
    size_t i = sizeof(var_types);
    while (i--) types[var_types[i]]++;
}

// allways starts with an identifier
tok_t* lex_statement(proj_t* proj) {
    tok_t* statement;
    if (!(statement=lex_ident(proj))&&!(statement=lex_operator(proj)))
        return NULL;
    if (statement->type==kw_local) {
        free(statement);
        if (!(statement=lex_ident(proj)))
            return NULL;
        else ++statement->local;
    }

    #define tok_(i) ((size_t)statement->contents[1]>i?statement->contents[i+2]:NULL)
    tok_t* tok;

    if (statement->type==oper_cbracket_close) goto ret;
    if (statement->type==oper_cbracket_open) {
        statement->type = statement_block;
        tok_t* sfst = allocpy(&(tok_t){.type=sfs}, sizeof(tok_t));
        size_t index = index_append((void***)&statement->file->tstack, sfst);
        debug(index);

        while ((tok=lex_statement(proj))&&tok->type!=oper_cbracket_close)
            index_append((void***)&statement->contents, tok);
        
        ((size_t*)statement->file->tstack)[1] = index;
        // TODO: INSANELY IMPORTANT memset the space to 0 afterwards
        memset(((void**)statement->file->tstack)+2+index, 0, (((size_t*)statement->file->tstack)[0]-index)*sizeof(void*));
        free(sfst);

        goto ret;
    }

    index_append((void***)&statement->contents, allocpy(statement, sizeof(tok_t)));
    if ((tok=lex_ident(proj))) index_append((void***)&statement->contents, tok);
    if ((tok=lex_operator(proj))) index_append((void***)&statement->contents, tok);

    // TODO: account for: var <oper>
    if (tok_(0)->type==var_ident) {
        // log("ADASASD COLLISION");


        goto ret;
    }
    // type var = value
    //          ^^^^^^^ - optional for var declaration
    // type var ()
    //          ^^ - func declaration (shouldn't be handled by var declaration)
    else if (!tok_(2)||tok_(2)->type != oper_rbracket_open) {
        if (types[tok_(0)->type]&&tok_(1)&&tok_(1)->type==var_ident) {
            bool invalid_declaration = 1;
            if (tok_(1)->file->tstack) {
                size_t i = (size_t)tok_(1)->file->tstack[1];
                while (i--) {
                    tok_t* t = tok_(1)->file->tstack[i+2];
                    if (t->type==var_decl&&!strncmp(t->start, tok_(1)->start, min(t->len, tok_(1)->len)))
                        invalid_declaration=0;
                    else if (t->type==type_decl&&!strncmp(t->start, tok_(1)->start, min(t->len, tok_(1)->len)))
                        invalid_declaration=0;
                    else if (t->type==func_decl&&!strncmp(t->start, tok_(1)->start, min(t->len, tok_(1)->len)))
                        invalid_declaration=0;
                    if (t->type==sfs||!invalid_declaration) break;
                }
            }
            if (invalid_declaration) {
                log("error: identifier '%.*s' is already in use, can't declare twice", (int)tok_(1)->len, tok_(1)->start);
                goto error;
            }
        }
        if (types[tok_(0)->type]&&tok_(2)&&tok_(2)->type==oper_assign) {
            tok_t* val = lex_value(proj);
            if (!val) {
                log("value was expected");
                goto error;
            }
            index_append((void***)&statement->contents, val);
            tok_t* semicolon = lex_operator(proj);
            if (semicolon&&semicolon->type!=oper_semicolon)
                undo(proj, semicolon);

        }
        if (tok_(1)->type==ident) {
            statement->file = tok_(1)->file;
            statement->len = tok_(1)->len;
            statement->line = tok_(1)->line;
            statement->line_start = tok_(1)->line_start;
            statement->start = tok_(1)->start;
            statement->type = var_decl;
        }
        goto ret;
    }

    switch (tok_(0)->type) {
    case kw_func: // func declaration
        

        break;
    
        default: log("ident was expected, got: %d", tok_(0)->type); goto error;
    }
ret:
    // TODO: set the statement not to the type but it's value
    if (!statement->local) index_append((void***)&file_top(proj)->globals, statement);
    return (index_append((void***)&file_top(proj)->locals, statement), statement);
error:
    while ((tok = index_pop((void***)&statement->contents))) free(tok);
    free(statement->contents);
    free(statement);
    return NULL;
}