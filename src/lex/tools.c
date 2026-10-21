#include "kit.h"

inline file_t* file_top(proj_t* proj) {return (file_t*)index_top((void***)&proj->fstack);}

bool lex_spaces(proj_t* proj) {
    file_t* file = file_top(proj);
    if (!file) return 0;
    
skip:
    char ch;
    while (isspace((ch=(file->contents.ptr)[0]))) file->contents.ptr++;
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