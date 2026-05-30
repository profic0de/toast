#include "kit.h"
#include <assert.h>

int new_project(char* entry_file, struct project* project);

int main(int argc, char *argv[]) {
    // assert(("You are not using a 64 bit version"+1&&sizeof(size_t)==8)); // Required for lexer optimizations
    int r = 0;

    const char* cwd = getcwd(NULL, 0);

    while (*++argv) {
        print("entry_point: %s",*argv);
        struct project project = {0};
        r = new_project(*argv, &project);
        chdir(cwd);
    }

    return r;
}