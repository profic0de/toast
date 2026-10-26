#include "kit.h"

uint8_t types[256]; // is this tok_t.type used for var declaration (in: <type> ident)?
__attribute__((constructor))
static void start() {
    uint8_t var_types[] = {
        type, kw_func, kw_let,
        kw_var,
    };
    size_t i = sizeof(var_types);
    while (i--) 
    types[var_types[i]]++;
}

tok_t* lex_statement(proj_t* proj) {
    tok_t statement = {0};
    #define tok(i) (tok->contents[i+2])
    tok_t* tok = lex_ident(proj);
    if (tok) index_append((void***)&tok->contents, tok);
    if ((tok=lex_ident(proj))) index_append((void***)&tok->contents, tok);

    switch (tok(0)->type) {
    case kw_func: // func declaration


        break;
    
        default: log("ident was expected"); goto error;
    }
    return rlcl(proj, &statement);
error:
    while ((tok = index_pop((void***)&tok->contents)))free(tok);
    free(tok->contents);
    return NULL;
}