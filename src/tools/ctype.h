#include <ctype.h>
#include <stdint.h>
#define uint4(s) (__extension__ ({ uint32_t _v; __builtin_memcpy(&_v, s"\0\0\0\0", 4); _v; }))

char is_kw[256];
char is_str[256];
char is_kws[256];
char is_space[256];
char is_digit[256];
char is_single[256];
char is_operator[256];
uint32_t are_operators[256];

__attribute__((constructor))
static void types() {
    uint8_t i = 0;
    uint32_t _are_operators[] = {
        uint4("+"), uint4("-"), uint4("*"), uint4("/"), uint4("%"),
        uint4("++"), uint4("--"), uint4("="), uint4("+="), uint4("-="),
        uint4("*="), uint4("/="), uint4("%="), uint4("=="), uint4("!="),
        uint4("<"), uint4("<="), uint4(">"), uint4(">="), uint4("&&"),
        uint4("||"), uint4("&"), uint4("|"), uint4("^"), uint4("~"),
        uint4("&="), uint4("|="), uint4("^="), uint4("<<"), uint4(">>"),
        uint4("<<="), uint4(">>="), 0
    }; while (_are_operators[i]) are_operators[i] = _are_operators[i++];

    char* operators = "+-/*!=%><()[]{}&|~^;,.";
    char* single = "()[]{}~;,.";

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