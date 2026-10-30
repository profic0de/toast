#include "kit.h"

inline file_t* file_top(proj_t* proj) {return (file_t*)index_top((void***)&proj->fstack);}
inline tok_t* rtok(proj_t* proj, tok_t* tok) {return (proj->last_tok = allocpy(tok, sizeof(tok_t)));}
// allocate tok and store it in locals of top file
inline tok_t* rlcl(proj_t* proj, tok_t* tok) {tok_t*t=allocpy(tok, sizeof(tok_t)); return (index_append((void***)&file_top(proj)->locals, t),t);}

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