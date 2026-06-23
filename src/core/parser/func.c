#include "kit.h"

int parse_func(struct parser* p) {
    
    if (!expect(p, IDENT)) return 1;

    return 0;
}
