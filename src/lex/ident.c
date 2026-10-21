#include "kit.h"

tok_t* lex_ident(proj_t* proj) {
    lex_spaces(proj);
    tok_t tok = {0};
    file_t* file = (tok.file = file_top(proj));
    if (!file) return NULL;
    tok.line = file->contents.newlines;
    tok.line_start = file->contents.line_start;
    if (!isalpha(file->contents.ptr[0])) return NULL;
    while (file->contents.ptr) {
        
    }
    
    return allocpy(&tok, sizeof(tok_t));
}