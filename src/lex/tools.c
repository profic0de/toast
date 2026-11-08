#include "kit.h"

inline file_t* file_top(proj_t* proj) {return (file_t*)index_top((void***)&proj->fstack);}
inline tok_t* rtok(proj_t* proj, tok_t* tok) {return (proj->last_tok = allocpy(tok, sizeof(tok_t)));}
// allocate tok and store it in locals of top file
inline tok_t* rlcl(proj_t* proj, tok_t* tok) {
    tok_t*t=allocpy(tok, sizeof(tok_t));
    if (!t->local) index_append((void***)&file_top(proj)->globals, t);
    return (index_append((void***)&file_top(proj)->locals, t),t);
}

bool lex_spaces(proj_t* proj) {
    file_t* file = file_top(proj);
    if (!file) return 0;
    
skip:
    char ch;
    while (isspace((ch=(file->contents.ptr)[0]))) {
        if (ch=='\n') file->contents.newlines += !!(file->contents.line_start=file->contents.ptr+1);
        file->contents.ptr++;
    }
    switch (ch) {
        case 0: return 0;
        case '#': {
            while (ch&&ch!='\n') ch=(++file->contents.ptr)[0];
            if (!ch) return 0;
            file->contents.newlines += !!(++file->contents.ptr); // skip '\n'
            file->contents.line_start = file->contents.ptr;
            goto skip;
        }
    }

    return 1;
}

void destroy(tok_t* tok) {
    if (!tok) return;
    tok_t* t;
    while ((t = index_pop((void***)&tok->contents))) destroy(t);
    free(tok->contents);
}

void unfold(tok_t* tok, size_t shift) {
    if (!tok) return;
    size_t s=shift, i=-1;
    while(s--) printf("\t");
    printf("%.*s (%d)\n", (int)tok->len, tok->start, tok->type);
    if (tok->contents)
        while (++i<=(size_t)tok->contents[0])
            unfold(tok->contents[i+2], shift+1);
}

// intended for simple tokens, may not work with complex ones
void undo(proj_t* proj, tok_t* tok) {
    if (!tok) return;
    file_t* file = file_top(proj);
    file->contents.ptr = tok->start;
    file->contents.newlines = tok->line;
    file->contents.line_start = tok->line_start;
    destroy(tok);
    if (index_top((void***)&file->locals)==tok)
        index_pop((void***)&file->locals);
}