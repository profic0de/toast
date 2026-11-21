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
    if (opers>1&&true) {
        uint8_t* powers = malloc(opers);
        size_t i = 0;
        while (i<opers) powers[i]=operators[val.contents[i*2+3]->type], i+=1;
        i=-1; tok_t** stack = 0, **contents=0; // pratt parsing: budget
        size_t power = 0;
        #define oper(i) val.contents[i*2+3]
        #define right(i) val.contents[i*2+2]
        #define left(i) val.contents[i*2+4]
        #define last ((size_t)stack[1]==0)
        while (++i<=opers) {
            if (i==opers||power>=powers[i]) {
                tok_t* tok=0; size_t index;
                while ((index=(size_t)index_pop((void***)&stack))) { index--;
                    if (!tok) {
                        tok = allocpy(&(tok_t){.type=value}, sizeof(tok_t));
                        index_append((void***)&tok->contents, right(index));
                        index_append((void***)&tok->contents, oper(index));
                        index_append((void***)&tok->contents, left(index));
                    } else {
                        tok_t* right = tok;
                        tok = allocpy(&(tok_t){.type=value}, sizeof(tok_t));
                        index_append((void***)&tok->contents, right);
                        index_append((void***)&tok->contents, oper(index));
                        index_append((void***)&tok->contents, left(index));
                    }
                    if (last) index_append((void***)&contents, tok);
                    
                    // debug(index);
                }
                power = 0;

            }
            if (i==opers) break;
            index_append((void***)&stack, (void*)i+1);
            power = powers[i];
        }
        free(val.contents);
        free(powers);
        free(stack);
        val.contents = contents;
        
    }

    val.type = value;
    return rtok(&val);
}
