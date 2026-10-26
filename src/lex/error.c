#include "kit.h"

void* lexer_error(proj_t* proj, char* msg) {
    if (!proj->last_tok) return NULL;
    proj->last_tok->line++;

    printf("%s:%lu:%lu %s\n", proj->last_tok->file->path, proj->last_tok->line, proj->last_tok->len?:1, msg);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    // printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);
    
    const char asd[] = " %lu | %.*s";
    size_t space = w.ws_col - sizeof(asd) - 1;
    char* sws = proj->last_tok->start-(space/5); {
        char* sws = proj->last_tok->start-(space/5);
        space = 0;
        while (sws[0]&&sws[0]!='\n') space += !!(sws++);
    }
    if (sws<proj->last_tok->start) sws = proj->last_tok->start;
    printf(asd, proj->last_tok->line, (int)space, sws);
    putc('\n', stdout);

    return NULL;
}