#include "kit.h"
#include <assert.h>

char str_box[UINT16_MAX+1];
char* cwd; size_t cwdl;
int new_project(char* entry_file, struct project* project);

int main(int argc, char *argv[]) {
    // assert(("You are not using a 64 bit version"+1&&sizeof(size_t)==8)); // Required for lexer optimizations
    int r = 0;
    cwd = getcwd(0,0);
    if (!cwd) return (perror("getcwd"), 1);
    cwdl = strlen(cwd);

    while (*++argv) {
        print("entry_point: %s",*argv);
        struct project project = {0};
        r = new_project(*argv, &project);
        hashmap_free(project.files);
    }

    return r;
}