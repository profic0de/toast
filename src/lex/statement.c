#include "kit.h"

tok_t* lex_statement(proj_t* proj) {
    tok_t tok = {0};

    return allocpy(&tok, sizeof(tok_t));
}