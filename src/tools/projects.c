#include "kit.h"

int parse_file(size_t fd, struct project* project);

int load_file(string filepath, struct project* project) {
    memset(str_box, 0, filepath.len+2);
    memcpy(str_box+1, filepath.text, filepath.len);

    char* filename = str_box+filepath.len;
    while (*--filename&&*filename!='/'); filename++;

    int len = str_box+1+filepath.len-filename;
    byte shift = 0; if (str_box[filepath.len]!='/') shift++;
    char full_path[filepath.len+shift+4+len]; memset(full_path, 0, filepath.len+shift+4+len);
    memcpy(full_path+1, filepath.text, filepath.len);

    byte ret = 1;

    struct stat sb;
    if (stat(str_box+1, &sb) == -1) return 1;

    if (!S_ISREG(sb.st_mode)&&ret++) {
        if (!S_ISDIR(sb.st_mode)) return ret; // Package not found

        full_path[filepath.len+shift] = '/';
        memcpy(full_path+filepath.len+shift+1, filename, len);
        memcpy(full_path+filepath.len+shift+1+len, ".t", 2);
    }

    struct file* file = (struct file*)hashmap_get(project->files, &(struct file){
        .path={.text=full_path+1, .len=(len=strlen(full_path+1))}
    });
    if (file) return 0;
    if (ret==2) print("package: [%s]",full_path+1);
    else print("file: [%s]",full_path+1);

    if (stat(full_path+1, &sb) == -1) return ret;
    if (!S_ISREG(sb.st_mode)) return ret;

    int fd = open(full_path+1, O_RDONLY);
    if (fd==-1) {
        print("failed to open %s",full_path+1);
        return 3;
    }

    hashmap_set(project->files, &(struct file){
        .name={.text=auto_free(strndup(filename,len)), .len=len},
        .path={.text=auto_free(strndup(full_path+1, len)), .len=len}
    });

    file = (struct file*)hashmap_get(project->files, &(struct file){
        .path={.text=full_path+1, .len=len}
    });

    if (!file) return ret;

    project->lf = file;

    int r = parse_file(fd, project);

    close(fd);
    return r;
}

static int comp(const void *a, const void *b, void *udata) {
    const struct file *fa = a;
    const struct file *fb = b;
    if (fa->path.len!=fb->path.len) return 1;
    return strncmp(fa->path.text, fb->path.text, fa->path.len);
}

static uint64_t hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct file *file = item;
    return hashmap_sip(file->path.text, file->path.len, seed0, seed1);
}

int new_project(char* entry_file, struct project* project) {
    project->files = hashmap_new(sizeof(struct file), 0,0,0, hash, comp, NULL, NULL);
    int r = load_file((string){.text=entry_file,.len=strlen(entry_file)}, project);
    return r;
}