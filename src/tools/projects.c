#include "kit.h"

int parse_file(size_t fd, struct project* project);

int cd(char* path, struct project* project) {
    char* ptr = path;
    if (path[0]=='/') chdir((ptr++,"/"));
    while (*ptr) {
        char* start = ptr;
        while (*ptr&&*ptr!='/') ptr++;
        int len = ptr-start;
        if (len) {
            print("%.*s",len,ptr-len);
        }
        if (*ptr) ptr++;
    }
    
    return 0;
}

int load_file(char* filename, struct project* project) {
    struct stat sb;
    if (stat(filename, &sb) == -1) return 1;
    print("file: [%s]",filename);

    struct folder* pwd = project->cwd;
    char* cwd = getcwd(NULL, 0);

    char d = 0;

    if (!S_ISREG(sb.st_mode)) {
        if (!S_ISDIR(sb.st_mode)) return (free(cwd),1); // Package not found

        cd(filename, project);
        d++;
    }

    char* path_end = filename, *ptr = filename-1;
    while (*++ptr) if (*ptr=='/') path_end=ptr+1;
    if (S_ISDIR(sb.st_mode)&&!d) {
        int len = path_end-filename;
        char path[len+1]; path[len] = '\0';
        memcpy(path, filename, len);

        cd(path, project);
    }

    char* file = path_end;

    if (stat(file, &sb) == -1) return (free(cwd),1);
    if (!S_ISREG(sb.st_mode)) return (free(cwd),1);

    size_t fd = open(file, O_RDONLY);
    if (fd==-1) {
        print("failed to open %s",file);

        project->cwd=pwd;
        chdir(cwd);
        free(cwd);
        return 2;
    }

    int r = parse_file(fd, project);

    close(fd);
    project->cwd=pwd;
    chdir(cwd);
    free(cwd);
    return r;
}

int new_project(char* entry_file, struct project* project) {
    memset(project, 0, sizeof(struct project));
    int r = load_file(entry_file, project);
    return r;
}