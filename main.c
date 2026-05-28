#include "kit.h"
#include <assert.h>

int file_store(char* filename);

int main(int argc, char *argv[]) {
    // assert(("You are not using a 64 bit version"+1&&sizeof(size_t)==8)); // Required for lexer optimizations
    int r = 0;

    while (*++argv) {
        print("entry_point: %s",*argv);
        r = file_store(*argv);
    }

    return r;
}