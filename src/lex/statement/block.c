#include "statement.h"

void lex_block(proj_t* proj) {
    tok_t* statement = proj->last_tok, *tok;
    statement->type = statement_block;
    tok_t* sfst = allocpy(&(tok_t){.type=sfs}, sizeof(tok_t));
    size_t index = index_append((void***)&statement->file->tstack, sfst);

    while ((tok=lex_statement(proj))&&tok->type!=oper_cbracket_close)
        index_append((void***)&statement->contents, tok);
    free(tok);
    if (!tok) lexer_error(statement, "A block was opened but never closed");

    ((size_t*)statement->file->tstack)[1] = index;

    memset(((void**)statement->file->tstack)+2+index, 0, (((size_t*)statement->file->tstack)[0]-index)*sizeof(void*));
    free(sfst);
}