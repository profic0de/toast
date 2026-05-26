#include "kit.h"
#include <assert.h>

int file_store(char* filename);

struct file** files;

int main(int argc, char *argv[]) {
    assert(("You are not using a 64 bit version"&&sizeof(size_t)==8)); // Required for lexer optimizations
    assert(("At least one argument required",argc>1));

    int r = file_store(argv[argc-1]);
    return r;
}