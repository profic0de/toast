#include "kit.h"

struct token next_token(char* buffer, char* end) {
    static char* list = "+-/*!=%%><()[]{}&|~^;,.";
    static char operators[256];
    if (operators[(int)*list]) goto skip;
    char* _list = list-1;
    while (*++_list) operators[(int)*_list]++;
    skip:

    while (isspace(*buffer));

    struct token token;
    token.buffer=buffer;
    token.type=NONE;
    token.len=0;

    return token;
}

__attribute__((destructor))
static void clean() {
    free(files);
}