#ifndef STATEMENT_H
#define STATEMENT_H

#include "kit.h"
#define tok_(i) ((size_t)statement->contents[1]>i?statement->contents[i+2]:NULL)
tok_t* lex_decl(proj_t* proj);

#endif