#include "tools/ctype.h"
#include "kit.h"

struct token next_token(char* buffer, char* end) {
    while (is_space((unsigned char)*buffer)) buffer++;
    if (buffer==end) return (struct token){.buffer=buffer,.type=EOF,.len=0};

    struct token token;
    token.buffer=buffer;
    token.type=EOF;
    token.len=0;

    return token;
}
