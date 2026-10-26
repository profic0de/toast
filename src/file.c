#include "kit.h"

file_t* open_file(proj_t* proj, char* path) {
    if (!path) return NULL;
    int fd = open(path, O_RDONLY);
    if (fd == -1) return NULL;
    size_t len = lseek(fd, 0, SEEK_END); lseek(fd, 0, SEEK_SET);
    file_t f = {0};
    f.path = realpath(path, NULL);
    file_t** files = proj->files;
    if (files) {
        size_t flen = (size_t)files[1]+2, i = 1;
        while (++i < flen) {
            if (files[i] && strcmp(files[i]->path, f.path)) continue;

            // log("duplicate");

            free(f.path);
            return files[i];
        }
    }

    f.fd = fd;
    f.contents.len = len;
    f.contents.buf = calloc(1, len+1);
    if (len>read(fd, f.contents.buf, len)) return (free(f.contents.buf), NULL);
    f.contents.ptr = f.contents.buf;

    // lex here
    file_t* file = allocpy(&f, sizeof(f));
    index_append((void***)&proj->files, file);
    index_append((void***)&proj->fstack, file);

    tok_t* statement;
    while ((statement = lex_statement(proj)));

    return index_pop((void***)&proj->fstack);
}