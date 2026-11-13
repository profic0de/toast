#include "value.h"

tok_t* lex_value(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t val = {0}, *tok; file_t* file;
    if (!(file=(val.file=file_top(proj)))) return NULL;
    val.line=file->contents.newlines;
    val.line_start=file->contents.line_start;
    char* ptr = (val.start=file->contents.ptr);
    if (isdigit(ptr[0])) tok=lex_num(proj);

    return tok;
}
