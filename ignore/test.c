#include "kit.h"
void print_bits(uint64_t v) {
    uint64_t mask = 1ULL<<63;
    uint8_t i = -1;
    while (++i<64) {
        if (!(i%8)) putc(' ', stdout);
        putc(v&mask?'1':'0', stdout);
        mask >>= 1;
    }
    putc('\n', stdout);
}
int main() {
    char* str = "wsdapcwsda";
    size_t kws[] = {
        s8("func"),
    };
    debug((char*)kws);

    size_t len = 2;
    size_t t = 0;
    memcpy(&t, str, len<8?len:8);
    // t<<=(8-len)*8;
    // t &= (~((size_t)(-1)>>((len)*8)));
    debug((char*)&t);
    print_bits(t);
    
    return 0;
}