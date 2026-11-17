#include "value.h"

tok_t* lex_value(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t val = {0}, *tok; file_t* file;
    if (!(file=(val.file=file_top(proj)))) return NULL;
    val.line=file->contents.newlines;
    val.line_start=file->contents.line_start;
    char* ptr = (val.start=file->contents.ptr);
    if (isdigit(ptr[0])||ptr[0]=='.') tok=lex_num(proj);
    else if (isalpha(ptr[0])) {
        tok=lex_ident(proj);
        if (tok->type==type_ident||tok->type==type)
            lexer_error(tok, "A type cannot be a value");
    } else if ((tok=lex_operator(proj))) {
        if (tok->type==oper_rbracket_open) {
            tok_t* value = lex_value(proj);
            if (!value) lexer_error(tok, "A value was expected afterwards");
            index_append((void***)&tok->contents, value);
            if (!(value=lex_operator(proj))||value->type!=oper_rbracket_close)
                lexer_error(tok, "The round bracket wasn't closed");
            free(value);

        } else return (free(tok), NULL);
    } else return NULL;

    return tok;
}
