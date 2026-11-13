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
    tok_t* statement;
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
    // if ((tok=lex_ident(proj))) index_append((void***)&statement->contents, tok);
    // if ((tok=lex_operator(proj))) index_append((void***)&statement->contents, tok);
    if (types[statement->type]&&lex_decl(proj)) goto ret;

ret:
    size_t i = statement->file->tstack?(size_t)statement->file->tstack[1]:0;
    // debug(i);
    if (i==0) {
        if (!statement->solid) index_append((void***)&file_top(proj)->globals, statement);
        return (index_append((void***)&file_top(proj)->locals, statement),statement);
    } else return (index_append((void***)&file_top(proj)->tstack, statement),statement);
}