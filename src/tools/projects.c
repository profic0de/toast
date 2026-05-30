#include "kit.h"

int parse_file(size_t fd);

int cd(char* path, struct project* project) {
    char* ptr = path;
    while (*ptr) {
        char* start = ptr;
        while (*ptr&&*ptr!='/') ptr++;
        if (ptr-start==2&&start[0]==start[1]&&start[0]=='.') project->cwd=project->cwd->parent;
        else if (ptr-start==1&&start[0]=='.');
        else if (ptr-start==0);
        else {
            // start is dir name start
            // len = ptr-start
        }
    }
    
    return 0;
}

int load_file(char* filename, struct project* project) {
    struct stat sb;
    if (stat(filename, &sb) == -1) return 1;
    if (!S_ISREG(sb.st_mode)) return 2; // Not a file

    char* path_end = filename, *ptr = filename-1; //null terminated

    while (*++ptr) if (*ptr=='/') path_end=ptr;

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