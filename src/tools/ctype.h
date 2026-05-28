#include <ctype.h>
#include <stdint.h>

char is_kw[256];
char is_kws[256];
char is_space[256];
char is_digit[256];
char is_single[256];
char is_operator[256];

__attribute__((constructor))
static void types() {
    char* operators = "+-/*!=%><()[]{}&|~^;,.";
    char* single = "()[]{}~;,.";
    uint8_t i = 0;

    i=0; while (++i) is_space[i] = !!isspace(i);
    i=0; while (++i) is_digit[i] = !!isdigit(i);
    i=0; while (++i) is_kws[i] = isalpha(i)||i=='_';
    i=0; while (++i) is_kw[i] = isalnum(i)||i=='_';

    while (*single) is_single[(int)*single++]++;
    while (*operators) is_operator[(int)*operators++]++;
}

#define is_kw(c) is_kw[c]
#define is_kws(c) is_kws[c]
#define is_space(c) is_space[c]
#define is_digit(c) is_digit[c]
#define is_single(c) is_single[c]
#define is_operator(c) is_operator[c]