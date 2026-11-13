#include "value.h"

bool hexc[256];
__attribute__((constructor))
static void test() {
    uint8_t* in = (uint8_t*)"abcdef0123456789ABCDEF"-1;
    while (*++in) hexc[*in]=1;
}

tok_t* lex_num(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t num = {0}; file_t* file;
    if (!(file=(num.file=file_top(proj)))) return NULL;
    num.line=file->contents.newlines;
    num.line_start=file->contents.line_start;
    num.len=1; num.type=numeric_value;
    char* ptr = (num.start=file->contents.ptr);
    while (isalnum(ptr[0])) ptr++;
    size_t len = ptr-num.start, i=-1;
    num.len = len, file->contents.ptr=ptr, ptr = num.start;
    if (len>=2&&ptr[0]=='0'&&isalpha(ptr[1])) {
        char hex = ptr[1];
        if (ptr[1]=='x'||ptr[1]=='b') {
            len-=2, ptr+=2;
            while (++i<len) {
                if (hex=='b'&&!(ptr[i]=='1'||ptr[i]=='0')) lexer_error(&num, "Invalid number");
                if (hex=='x'&&!(hexc[(uint8_t)ptr[i]])) lexer_error(&num, "Invalid number");
            }
        }
        else lexer_error(&num, "");
    } else while (++i<len) if (!isdigit(ptr[i]))
        lexer_error(&num, "Invalid number");
    
    return rtok(&num);
}