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
    printf("%.*s\n", 2, "asdasd");

    return 0;
}