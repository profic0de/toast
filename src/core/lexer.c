#include "tools/ctype.h"
#include "kit.h"

struct token next_token(char* buffer, char* end) {
    while (is_space((unsigned char)*buffer)) buffer++;
    if (buffer==end) return (struct token){.buffer=buffer,.type=EOF,.len=0};
    uint8_t c = *buffer++;

    if (c=='#') {if (end-buffer<8) while (*buffer&&*buffer++!='\n'); else {
        uint64_t val; memcpy(&val, buffer, 8);
        if (val==uint("require ")) print("pass");
        else while (*buffer&&*buffer++!='\n');
    }} if (is_single(c)) return (struct token){.type=SYMBOL,.buffer=buffer,.len=1};

    struct token token;
    token.buffer=buffer;
    token.type=EOF;
    token.len=0;

    return token;
}
