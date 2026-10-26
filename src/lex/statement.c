#include "kit.h"

tok_t* lex_statement(proj_t* proj) {
    tok_t statement = {0};
    tok_t** tstack = NULL;
    #define tok(i) tstack[i+2]
    tok_t* tok = lex_ident(proj);
    tok->contents = tstack;
    if (tok) index_append((void***)&tstack, tok);
    switch (tok(0)->type) {
    case kw_func: // func declaration or variable declaration (should handle both)


        break;
    
        default: log("ident was expected"); goto error;
    }
    return allocpy(&statement, sizeof(tok_t));
error:
    while ((tok = index_pop((void***)&tstack)))free(tok);
    free(tstack);
    return NULL;
}