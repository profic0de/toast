#include <ctype.h>
#include <stdint.h>
#include "kit.h"
#define uint4(s) (__extension__ ({ uint32_t _v; __builtin_memcpy(&_v, s"\0\0\0\0", 4); _v; }))
#define uint8(s) (__extension__ ({ uint64_t _v; __builtin_memcpy(&_v, s"\0\0\0\0\0\0\0\0", 8); _v; }))

char is_kw[256];
char is_str[256];
char is_kws[256];
char is_space[256];
char is_digit[256];
char is_single[256];
char is_operator[256];
uint64_t are_keywords[256];
uint32_t are_operators[256];
extern uint8_t is_type[TOKEN_TYPES];

__attribute__((constructor))
static void types() {
    uint8_t i = 0;
    uint8_t _are_types[] = {IDENT, KW_VAR, KW_LET, KW_FUNC, 0};
    while (_are_types[i]) {is_type[i] = _are_types[i]; i++;};

    i=0; uint32_t _are_operators[] = {
        uint4("+"), uint4("-"), uint4("*"), uint4("/"), uint4("%"),
        uint4("++"), uint4("--"), uint4("="), uint4("+="), uint4("-="),
        uint4("*="), uint4("/="), uint4("%="), uint4("=="), uint4("!="),
        uint4("<"), uint4("<="), uint4(">"), uint4(">="), uint4("&&"),
        uint4("||"), uint4("&"), uint4("|"), uint4("^"), uint4("~"),
        uint4("&="), uint4("|="), uint4("^="), uint4("<<"), uint4(">>"),
        uint4("<<="), uint4(">>="), 0
    };
    while (_are_operators[i]) {are_operators[i] = _are_operators[i]; i++;};

    i=0; uint64_t _are_keywords[] = {
        uint8("var"), uint8("func"), uint8("let"), uint8("obj"), uint8("self"), 
        uint8("return"), uint8("break"), uint8("if"), uint8("wait"), uint8("yield"), 
        uint8("while"), 0
    };
    while (_are_keywords[i]) {are_keywords[i] = _are_keywords[i]; i++;};

    char* operators = "+-/*!=%><()[]{}&|~^;,.";
    char* single = "()[]{};,.:";

    i=0; while (++i) is_digit[i] = !!isdigit(i);
    i=0; while (++i) is_space[i] = !!isspace(i);
    i=0; while (++i) is_kws[i] = isalpha(i)||i=='_';
    i=0; while (++i) is_str[i] = (i!='\\'&&i!='"');
    i=0; while (++i) is_kw[i] = isalnum(i)||i=='_';

    i=0; while (*single) is_single[(int)*single++]=++i;
    i=0; while (*operators) is_operator[(int)*operators++]=++i;
}

#define is_kw(c) is_kw[c]
#define is_kws(c) is_kws[c]
#define is_str(c) is_str[c]
#define is_space(c) is_space[c]
#define is_digit(c) is_digit[c]
#define is_single(c) is_single[c]
#define is_operator(c) is_operator[c]