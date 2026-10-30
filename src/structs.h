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
        oper,
            oper_semicolon, 
            oper_rbracket_open, oper_rbracket_close, // round brackets '()'
            oper_sbracket_open, oper_sbracket_close, // square brackets '[]'
            oper_cbracket_open, oper_cbracket_close, // curly brackets '{}'

            oper_incr, oper_incr_early, oper_incr_late,
            oper_addition, oper_substraction, 
            oper_decr, oper_decr_early, oper_decr_late,
            oper_equals, oper_assign,
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