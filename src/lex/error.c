#include "kit.h"

void* lexer_error(tok_t* tok, char* msg) {
    if (!tok) return NULL;
    tok->line++;
    char* cwd = getcwd(NULL, 0);
    while (*cwd++==*tok->file->path++);

    printf("[%s:%lu:%lu]: %s\n", tok->file->path, tok->line, tok->start-tok->line_start+1, msg);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    // printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);
    
    const char asd[] = " %lu | %.*s\n";
    int space = w.ws_col - sizeof(asd) + 5, len=0;
    char* sws = tok->start-(space/5);
    size_t prefix_len = tok->start-tok->line_start, digits=1;
    if (true) {
        size_t n = tok->line; while (n/=10) digits++;

        sws = tok->start-min(prefix_len, (space/5)), len = min(prefix_len, (space/5));
        char* sws = tok->start;
        while (sws[0]&&sws[0]!='\n') len += !!(sws++);
        len = min(len, space);
    }

    printf(asd, tok->line, (int)(len-digits+1), sws);
    len = digits+2; while (len--) putc(' ', stdout); putc('|', stdout);
    
    len = min(prefix_len, (space/5))+1;
    while (len--) putc(' ', stdout);
    puts("^\n");

    longjmp(tok->file->proj->jump[0], 1);
    return NULL;
}