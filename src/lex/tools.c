#include "kit.h"

inline file_t* file_top(proj_t proj) {return (file_t*)index_top((void***)&proj.fstack);}

bool lex_spaces(proj_t proj) {
    file_t* file = file_top(proj);
    if (!file) return 0;
    
    

    return 1;
}