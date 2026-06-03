#include "kit.h"

int parse_file(size_t fd, struct project* project);

int load_file(char* filepath, struct project* project) {
    struct stat sb;
    if (stat(filepath, &sb) == -1) return 1;

    char d = 0;

    if (!S_ISREG(sb.st_mode)) {
        if (!S_ISDIR(sb.st_mode)) return 1; // Package not found

        print("package: [%s]",filepath);
        d++;
    }
    else print("file: [%s]",filepath);

    char* path_end = filepath, *ptr = filepath-1;
    while (*++ptr) if (*ptr=='/') path_end=ptr+1;
    if (S_ISDIR(sb.st_mode)&&!d) {
        int len = path_end-filepath;
        char path[len+1]; path[len] = '\0';
        memcpy(path, filepath, len);
    }

    int len = strlen(path_end)+1;

    char file[len+3];
    file[len+0] = '.';
    file[len+1] = 't';
    file[len+2] = '\0';

    strncpy(file, path_end, len);

    if (stat(file, &sb) == -1) return 1;
    if (!S_ISREG(sb.st_mode)) return 1;

    int fd = open(file, O_RDONLY);
    if (fd==-1) {
        print("failed to open %s",file);
        return 2;
    }

    int r = parse_file(fd, project);

    close(fd);
    return r;
}

static int comp(const void *a, const void *b, void *udata) {
    const struct file *fa = a;
    const struct file *fb = b;
    if (fa->path.len!=fb->path.len) return 1;
    return strcmp(fa->path.text, fb->path.text);
}

static uint64_t hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct file *file = item;
    return hashmap_sip(file->path.text, file->path.len, seed0, seed1);
}

int new_project(char* entry_file, struct project* project) {
    project->files = hashmap_new(sizeof(struct file), 0,0,0, hash, comp, NULL, NULL);
    int r = load_file(entry_file, project);
    return r;
}