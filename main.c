#include "kit.h"
#include <assert.h>

int file_store(char* filename);
int run_file(int idx);

int main(int argc, char *argv[]) {
    assert(("You are not using a 64 bit os"&&sizeof(size_t)==8)); // Required for lexer optimizations
    assert(("At least one argument required",argc>1));

    int r = file_store(argv[argc-1]);
    return r;
}