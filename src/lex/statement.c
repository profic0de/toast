#include "kit.h"

static uint8_t types[256]; // is this tok_t.type used for var declaration (in: <type> ident)?
__attribute__((constructor))
static void start() {
    uint8_t var_types[] = {
        type, kw_func, kw_let,
        kw_var, type_decl
    };
    size_t i = sizeof(var_types);
    while (i--) types[var_types[i]]++;
}

// allways starts with an identifier
tok_t* lex_statement(proj_t* proj) {
    tok_t* statement;
    if (!(statement=lex_ident(proj))) return NULL;
    if (statement->type==kw_local) {
        free(statement);
        if (!(statement=lex_ident(proj)))
            return NULL;
        else ++statement->local;
    }

    #define tok_(i) ((size_t)statement->contents[1]>i?statement->contents[i+2]:NULL)
    tok_t* tok;
    index_append((void***)&statement->contents, allocpy(statement, sizeof(tok_t)));
    if ((tok=lex_ident(proj))) index_append((void***)&statement->contents, tok);
    if ((tok=lex_operator(proj))) index_append((void***)&statement->contents, tok);

    // TODO: account for: var <oper>
    if (tok_(0)->type==var_ident) {
        log("ADASASD COLLISION");


    }
    // type var = value
    //          ^^^^^^^ - optional for var declaration
    // type var ()
    //          ^^ - func declaration (shouldn't be handled by var declaration)
    else if (!tok_(2)||tok_(2)->type != oper_rbracket_open) {
        if (!tok_(2));
        else if (tok_(2)->type==oper_assign) {
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
    return NULL;
}