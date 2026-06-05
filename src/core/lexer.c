#include "tools/ctype.h"
#include "kit.h"

struct token next_token(char* buffer, char* end, struct project* project) {
again:
    while (is_space((unsigned char)*buffer)) buffer++;
    if (buffer==end) return (struct token){.buffer=buffer,.type=EOF,.len=0};
    uint8_t c = *buffer++;

    if (c=='#') {if (end-buffer<8) {while (*buffer&&*buffer++!='\n'); goto again;} else {
        uint64_t val; memcpy(&val, buffer, 8);
        if (val==uint("require ")) { buffer += 8;
            if (*buffer!='\''&&*buffer!='"'&&*buffer!='<') {while (*buffer&&*buffer++!='\n'); goto again;}

            return (struct token){.type=REQ,.buffer=buffer,.len=0};
        } else {while (*buffer&&*buffer++!='\n'); goto again;}
    }} if (is_single(c)) return (struct token){.type=SYMBOL,.buffer=buffer-1,.len=1};

    // print("empty");

    struct token token;
    token.buffer=buffer;
    token.type=EOF;
    token.len=0;

    return token;
}
