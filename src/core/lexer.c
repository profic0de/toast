#include "kit.h"

struct token next_token(char* buffer, char* end) {
    while (is_space(*buffer));

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