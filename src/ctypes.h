#include <ctype.h>
#include <stdint.h>

char operators[256];
char is_space[256];

__attribute__((constructor))
static void types() {
    static char* list = "+-/*!=%%><()[]{}&|~^;,.";

    uint8_t i = 1; while (i) is_space[i] = isspace(i++);
    char* _list = list-1; while (*++_list) operators[(int)*_list]++;
}

#define is_space(c)