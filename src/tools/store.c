#include "kit.h"

int parse_file(size_t fd);

int file_store(char* filename) {
    struct stat sb;
    if (stat(filename, &sb) == -1) {
        perror("stat");
        return 1;
    }

    if (!S_ISREG(sb.st_mode)) {
        print("%s is not a file",filename);
        return 2;
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
    file->requirements = NULL;

    files = array_append(files, file);

    int r = parse_file(fd);

    auto_free(file->requirements);

    close(fd);
    return r;
}