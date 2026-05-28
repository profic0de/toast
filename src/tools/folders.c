#include "kit.h"

int parse_file(size_t fd);

int load_file(char* filename, struct folder* project) {
    struct stat sb;
    if (stat(filename, &sb) == -1) return 1;
    if (!S_ISREG(sb.st_mode)) return 2; // Not a file

    char** path; //null terminated

    while (*filename) {
        
    }

    if (files) {
        struct file** temp = files-1;
        while (*++temp&&strcmp(temp[0]->filename,filename));
        if (*temp) return 0;
    }

    size_t fd = open(filename, O_RDONLY);
    if (!fd) {
        print("failed to open %s",filename);
        return 3;
    }

    struct file* file = auto_free(malloc(sizeof(struct file)));
    file->filename = auto_free(strdup(filename));

    files = array_append(files, file);

    int r = parse_file(fd);

    close(fd);
    return r;
}

int new_project(char* entry_file, struct folder* project) {
    int r = load_file(entry_file, project);
    return r;
}