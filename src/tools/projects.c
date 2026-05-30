#include "kit.h"

int parse_file(size_t fd);


int cd(char* path, struct project* project);

int load_file(char* filename, struct project* project) {
    struct stat sb;
    if (stat(filename, &sb) == -1) return 1;
    if (!S_ISREG(sb.st_mode)) return 2; // Not a file

    char** path; //null terminated

    while (*filename) {
        
    }

    size_t fd = open(filename, O_RDONLY);
    if (!fd) {
        print("failed to open %s",filename);
        return 3;
    }

    int r = parse_file(fd);

    close(fd);
    return r;
}

int new_project(char* entry_file, struct project* project) {
    int r = load_file(entry_file, project);
    return r;
}