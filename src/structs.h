#ifndef STRUCTS_H
#define STRUCTS_H

#include "kit.h"

typedef struct file file_t;
typedef struct token {
    char* ptr;
    size_t len;
    file_t* file;
    enum {
        eof, ident,
    } type;
} tok_t;

typedef struct file {
    int fd;
    struct {
        char* ptr;
        char* buf;
        size_t len;
    } contents;;
    tok_t globals;
    tok_t locals;
} file_t;

typedef struct project { //"env"
    file_t** fstack;
    tok_t* main_func;
} proj_t;



#endif