#include "kit.h"
#include <assert.h>

int file_store(char* filename);

struct file** files;

int main(int argc, char *argv[]) {
    // assert(("You are not using a 64 bit version"+1&&sizeof(size_t)==8)); // Required for lexer optimizations
    int r = 0;

    while (*++argv) {
        print("testing: %s",*argv);
        r = file_store(*argv);
    }

    return r;
}