#ifndef STRUCTS_H
#define STRUCTS_H

#include "kit.h"

typedef struct file file_t;
typedef struct token tok_t;
typedef struct token {
    char* start;
    size_t len;
    file_t* file;
    enum : uint8_t {
        // NOTE: Don't forget to update:
        //      > statement.c:6
        //      > ident.c:3
        eof, ident, type, sfs, // stack frame start
        kw, // oder sensitive
            kw_func, kw_return,
            kw_var, kw_let, kw_type,
    } type;
    size_t line;
    char* line_start;
    tok_t** contents;
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
    tok_t** tstack;
    tok_t* main_func;
    tok_t* last_tok;
} proj_t;

#endif