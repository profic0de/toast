#include <ctype.h>
#include <stdint.h>

char operators[256];
char is_space[256];

__attribute__((constructor))
static void types() {
    char* list = "+-/*!=%%><()[]{}&|~^;,.";

    uint8_t i = 0; while (++i) is_space[i] = isspace(i)?1:0;
    char* _list = list-1; while (*++_list) operators[(int)*_list]++;
}

#define is_space(c) is_space[c]
#define is_operator(c) operators[c]