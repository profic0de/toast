#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define uint4(s) (__extension__ ({ uint32_t _v; __builtin_memcpy(&_v, s"\0\0\0\0", 4); _v; }))
uint32_t are_operators[256];

__attribute__((constructor))
static void types() {
    uint8_t i = 0;
    uint8_t _are_operators[] = {
        uint4("+"), uint4("-"), uint4("*"), uint4("/"), uint4("%"),
        uint4("++"), uint4("--"), uint4("="), uint4("+="), uint4("-="),
        uint4("*="), uint4("/="), uint4("%="), uint4("=="), uint4("!="),
        uint4("<"), uint4("<="), uint4(">"), uint4(">="), uint4("&&"),
        uint4("||"), uint4("&"), uint4("|"), uint4("^"), uint4("~"),
        uint4("&="), uint4("|="), uint4("^="), uint4("<<"), uint4(">>"),
        uint4("<<="), uint4(">>="), 0
    }; 
    while (_are_operators[i]) {are_operators[i] = _are_operators[i]; i++;};
}

int main() {
    printf("%x\n",*are_operators);

    return 0;
}