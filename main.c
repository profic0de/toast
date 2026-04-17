#include "kit.h"

int main() {
    auto_free(malloc(1));
    auto_free(malloc(1));
    return 0;
}