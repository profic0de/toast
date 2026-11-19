#include "value.h"

static uint8_t operators[256];

__attribute__((constructor))
static void start() {
    uint8_t precedences[] = {
        1, oper_addition, oper_substraction,
        2,
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
    for (;;) {
        char* ptr = file->contents.ptr;
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

        tok = lex_operator(proj);
        if (tok&&operators[tok->type]) index_append((void***)&val.contents, tok);
        else { undo(tok); break; }
    }
    //TODO: Do some pratt parsing schenanigans or whatever this is called
    size_t opers = ((size_t)val.contents[1]-1)/2;
    if (opers>1) {
        uint8_t* powers = malloc(opers);
        size_t i = 0;
        while (i<opers) powers[i]=operators[val.contents[i*2+3]->type], i+=1;
        for (;;) {
            size_t max = 0, i = 0, b, nmax = 0;
            while (i<opers) {
                if (powers[i] >max) max = powers[i], nmax = 0;
                if (powers[i]==max) nmax += 1;
                i += 1;
            } b = 0;
            size_t j = 0;
            while (nmax--) {
                i = b; while (i<opers) if (powers[i]==max) break; else i += 1; b = i;
                tok_t** arr = val.contents+2;
                size_t i=0, k=b*2+1;
                while (i<=k) {
                    arr[j] = arr[i];
                    if (i==k) {
                        // TODO: Finish this
                        tok_t* tok = allocpy(&(tok_t){.type=value}, sizeof(tok_t));
                        index_append((void***)tok->contents, arr[i+0]);
                        index_append((void***)tok->contents, arr[i+1]);
                        index_append((void***)tok->contents, arr[i+2]);
                        i += 2;
                        arr[j] = tok;

                    }
                    i++, j++;
                }

                // debug(nmax);
            }
            debug(j);
        }
    }

    val.type = value;
    return rtok(&val);
}
