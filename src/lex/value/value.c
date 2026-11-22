#include "value.h"

static uint8_t operators[256];

__attribute__((constructor))
static void start() {
    uint8_t precedences[] = {
        1, oper_addition, oper_substraction,
        2, oper_mul, oper_div,
    };
    size_t val = 0, target = sizeof(precedences), i = 0;
    while (i<target) {
        if (precedences[i]<oper) val = precedences[i];
        else operators[precedences[i]] = val;
        i += 1;
    }
}

tok_t* lex_value(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t val = {0}, *tok; file_t* file;
    if (!(file=(val.file=file_top(proj)))) return NULL;
    val.line=file->contents.newlines;
    val.line_start=file->contents.line_start;
    val.start=file->contents.ptr;
    tok_t** opers = 0;
    for (;;) {
        if (!lex_spaces(proj)) return lexer_error(&val, "Unexpected EOF");
        char* ptr = file->contents.ptr;
        //TODO: Parse more stuff like functions arrays etc.
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

            } else lexer_error(tok, "Unexpected operator");
        } else return NULL;

        index_append((void***)&val.contents, tok);
        bool stop = false;
        tok = lex_operator(proj);
        if (tok&&operators[tok->type]);
        else undo(tok), stop++, tok=0;

        #define power(tok) (tok?operators[tok->type]:0)

        tok_t* last = index_top((void***)&opers);
        while (last&&power(last)>=power(tok)) {
            tok_t* node = allocpy(&(tok_t){.type=node_value}, sizeof(tok_t));
            tok_t* value = index_pop((void***)&val.contents);
            index_append((void***)&node->contents, index_pop((void***)&opers));
            index_append((void***)&node->contents, index_pop((void***)&val.contents));
            index_append((void***)&node->contents, value);
            index_append((void***)&val.contents, node);

            last = index_top((void***)&opers);
        }

        if (stop) break; 
        index_append((void***)&opers, tok);
    }
    free(opers);

    val.type = value;
    return rtok(&val);
}
