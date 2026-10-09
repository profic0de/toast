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
    uint64_t _b1 = 1UL<<63;
    uint64_t _b2 = 1UL<<63;
    uint8_t  bits = sizeof(_b1)*8;
    uint64_t mask = bits>=64?-1:( 1ULL << bits ) - 1;

    uint64_t v = 12314121231;
    print_bits(v);
    print_bits(~((1UL << 12)-1)&v);

    uint64_t sum = _b1 + _b2;
    if (sum>mask||_b1>sum) printf("overflow\n");

    print_bits(mask);
    print_bits(sum);

    return 0;
}