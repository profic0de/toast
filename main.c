#include "kit.h"
#include <assert.h>

char str_box[UINT16_MAX+1];
int new_project(char* entry_file, struct project* project);

int main(int argc, char *argv[]) {
    assert(PATH_MAX<sizeof(str_box)-2);
    // assert(("You are not using a 64 bit version"+1&&sizeof(size_t)==8)); // Required for lexer optimizations
    int r = 0;

    while (*++argv) {
        print("entry_point: %s",*argv);
        struct project project = {0};
        r = new_project(*argv, &project);
        hashmap_free(project.files);
    }

    return r;
}