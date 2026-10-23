#ifndef STRUCTS_H
#define STRUCTS_H

#include "kit.h"

typedef struct file file_t;
typedef struct token {
    char* start;
    size_t len;
    file_t* file;
    enum {
        eof, ident,
    } type;
    size_t line;
    char* line_start;
} tok_t;

typedef struct file {
    int fd;
    char* path;
    struct {
        char* ptr;
        char* buf;
        size_t len;
        size_t newlines;
        char* line_start;
    } contents;
    tok_t globals;
    tok_t locals;
} file_t;

typedef struct project { //"env"
    file_t** fstack;
    file_t** files;
    tok_t* main_func;
    tok_t* last_tok;
} proj_t;

#endif