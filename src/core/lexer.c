#include "kit.h"

#define POS ptr-buffer

struct token next_token(char* buffer, char* end) {
    static char* list = "+-/*!=%%><()[]{}&|~^;,.";
    static char operators[256];
    if (operators[(int)*list]) goto skip;
    char* _list = list-1;
    while (*++_list) operators[(int)*_list]++;
    skip:

    struct token token;
    token.buffer=buffer;
    token.type=NONE;

    return token;
}

__attribute__((destructor))
static void clean() {
    free(files);
}