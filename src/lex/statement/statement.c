#include "statement.h"

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

tok_t* lex_statement(proj_t* proj) {
    tok_t* statement, *tok;
    if (!(statement=lex_ident(proj))) {
        if (!(statement=lex_operator(proj))) return NULL;
        else if (statement->type==oper_cbracket_close) goto ret;
        else if (statement->type==oper_cbracket_open) {
            proj->last_tok = statement;
            void lex_block(proj_t* proj);
            lex_block(proj);
            goto ret;
        } else lexer_error(statement, "no idea what this is for");
    } if (statement->type==kw_solid) {
        free(statement);
        if (!(statement=lex_ident(proj)))
            return NULL;
        else ++statement->solid;
    }
    proj->last_tok = statement;

    index_append((void***)&statement->contents, allocpy(statement, sizeof(tok_t)));
    if ((tok=lex_ident(proj))) index_append((void***)&statement->contents, tok);
    if ((tok=lex_operator(proj))) index_append((void***)&statement->contents, tok);

    // TODO: account for: var <oper>
    if (tok_(0)->type==var_ident) {
        log("ADASASD COLLISION");


        goto ret;
    } else
    if (!tok_(2)||tok_(2)->type != oper_rbracket_open) {
        // runs if no operator or it's not '('
        bool invalid_declaration = 0;
        if (types[tok_(0)->type]&&tok_(1)&&tok_(1)->type==type_ident) {
            // runs if:
            // <type> <type name>
            log("error: identifier '%.*s' is already in use by a type, can't use it", (int)tok_(1)->len, tok_(1)->start);
            goto error;
        } else if (types[tok_(0)->type]&&tok_(1)&&tok_(1)->type==var_ident) {
            // runs if it's a kw that can be used for var declaration and
            // the var name is already used
            size_t i = tok_(1)->file->tstack?(size_t)tok_(1)->file->tstack[1]:0;
            if (i>0) {
                while (i--) {
                    tok_t* t = tok_(1)->file->tstack[i+2];
                    if (t->type==var_decl&&!strncmp(t->start, tok_(1)->start, min(t->len, tok_(1)->len)))
                        invalid_declaration=1;
                    else if (t->type==type_decl&&!strncmp(t->start, tok_(1)->start, min(t->len, tok_(1)->len)))
                        invalid_declaration=1;
                    else if (t->type==func_decl&&!strncmp(t->start, tok_(1)->start, min(t->len, tok_(1)->len)))
                        invalid_declaration=1;
                    if (t->type==sfs||invalid_declaration) break;
                }
            } else {
                i = (size_t)tok_(1)->file->locals[1];
                while (i--) {
                tok_t* t = tok_(1)->file->locals[i+2];
                if (t->type==var_decl&&!strncmp(t->start, tok_(1)->start, min(t->len, tok_(1)->len)))
                    invalid_declaration=1;
                else if (t->type==type_decl&&!strncmp(t->start, tok_(1)->start, min(t->len, tok_(1)->len)))
                    invalid_declaration=1;
                else if (t->type==func_decl&&!strncmp(t->start, tok_(1)->start, min(t->len, tok_(1)->len)))
                    invalid_declaration=1;
                if (invalid_declaration) break;}
            }
            i = tok_(1)->file->tstack?(size_t)tok_(1)->file->tstack[1]:0;
            // invalid_declaration = invalid_declaration&&i>1;
            if (invalid_declaration) {
                lexer_error(tok_(1), "error");
                log("error: identifier '%.*s' is already in use, can't declare twice", (int)tok_(1)->len, tok_(1)->start);
                goto error;
            }
            tok_t* semicolon = index_pop((void***)&statement->contents);
            if (semicolon&&semicolon->type!=oper_semicolon)
                undo(proj, semicolon);
            free(semicolon);
        } else if (types[tok_(0)->type]&&tok_(2)&&tok_(2)->type==oper_assign) {
            tok_t* val = lex_value(proj);
            if (!val) {
                log("value was expected");
                goto error;
            }
            index_append((void***)&statement->contents, val);
            tok_t* semicolon = lex_operator(proj);
            if (semicolon&&semicolon->type!=oper_semicolon)
                undo(proj, semicolon);
        } else if (tok_(2)) {
            tok_t* semicolon = index_pop((void***)&statement->contents);
            if (semicolon&&semicolon->type!=oper_semicolon)
                undo(proj, semicolon);
            free(semicolon);
        }
        if (tok_(1)&&!invalid_declaration&&(tok_(1)->type==ident||tok_(1)->type==var_ident||tok_(1)->type==func_ident)) {
            statement->file = tok_(1)->file;
            statement->len = tok_(1)->len;
            statement->line = tok_(1)->line;
            statement->line_start = tok_(1)->line_start;
            statement->start = tok_(1)->start;
            statement->type = var_decl;
        }
        goto ret;
    } else if (!tok_(2)||tok_(2)->type == oper_rbracket_open) {
        // TODO: Handle function declaration
    }
ret:
    size_t i = statement->file->tstack?(size_t)statement->file->tstack[1]:0;
    // debug(i);
    if (i==0) {
        if (!statement->solid) index_append((void***)&file_top(proj)->globals, statement);
        return (index_append((void***)&file_top(proj)->locals, statement),statement);
    } else return (index_append((void***)&file_top(proj)->tstack, statement),statement);
error:
    while ((tok = index_pop((void***)&statement->contents))) free(tok);
    free(statement->contents);
    free(statement);
    return NULL;
}