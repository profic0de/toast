#ifndef KIT_H
#define KIT_H
#define _GNU_SOURCE
// open, read, etc
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include "tools/hashmap/hashmap.h"

#define byte uint8_t
extern char str_box[UINT16_MAX+1];

typedef struct string {
    char* text;
    uint16_t len;
} string;

void* auto_free(void* ptr);
void str_append(char** str, char c);
int dict_append(char*** arr, char* ptr);
void** array_append(void** arr, void* ptr);
void error(const char* filename, size_t pos, size_t token_len, const char* fmt, ...);

#undef EOF
enum token_type {
    ERR,EOF,NUMBER,FLOAT,STRING,PATH,WORD,NSTRING,BSTRING,IDENT,
    KEYWORD,
        KW_VAR, KW_FUNC, KW_LET, KW_OBJ, KW_SELF, KW_RETURN, KW_BREAK, KW_IF, KW_WAIT,
        KW_YIELD, KW_WHILE, KW_REF,
    SYMBOL,
        LEFT_PAREN,RIGHT_PAREN,LEFT_BRACKET,RIGHT_BRACKET,LEFT_BRACE,RIGHT_BRACE,SEMICOLON,COMMA,DOT,COLON,
    OPERATOR,
        OPER_PLUS,OPER_MINUS,OPER_STAR,OPER_SLASH,OPER_PERCENT,
        OPER_INCREMENT,OPER_DECREMENT,
        OPER_ASSIGN,OPER_ADD_ASSIGN,OPER_SUB_ASSIGN,OPER_MUL_ASSIGN,OPER_DIV_ASSIGN,OPER_MOD_ASSIGN,
        OPER_EQUAL,OPER_NOT_EQUAL,
        OPER_LESS,OPER_LESS_EQUAL,OPER_GREATER,OPER_GREATER_EQUAL,
        OPER_LOGICAL_AND,OPER_LOGICAL_OR,
        OPER_BITWISE_AND,OPER_BITWISE_OR,OPER_BITWISE_XOR,OPER_BITWISE_NOT,
        OPER_AND_ASSIGN,OPER_OR_ASSIGN,OPER_XOR_ASSIGN,
        OPER_LEFT_SHIFT,OPER_RIGHT_SHIFT,
        OPER_LEFT_SHIFT_ASSIGN,OPER_RIGHT_SHIFT_ASSIGN,

    TOKEN_TYPES
};
struct token {
    enum token_type type;
    union {
        double f;
        uint64_t i;
    };
    char* start;
    size_t len;
};

#define array_append(arr, ptr) ((__typeof__(arr))array_append(((void**)(arr)), ((void*)(ptr))))
#include "core/ast.h"

extern uint8_t is_type[TOKEN_TYPES];

struct file {
    string name;
    string path;

    struct node ast; 
};

struct project {
    struct file* main_file;

    char** src_paths;
    char** lib_paths;
    struct hashmap* files;
    struct file* lf; // last file
};

struct parser {
    char** buffer;
    char* start;
    char* end;

    struct token tok;

    struct project* p;
    struct node* ast;
};

struct token next_token(char** buffer, char* start, char* end, struct project* project);
static inline struct token next(struct parser* p) {return (p->tok=next_token(p->buffer, p->start, p->end, p->p));}
static inline void back(struct parser* p) {*p->buffer=p->tok.start;}
static inline int expect(struct parser* p, enum token_type type) {
    struct token t = next(p);
    if (t.type != type) return (error(p->p->lf->path.text,t.start - p->start, t.len, "error: unexpected token"),0);
    return 1;
}

#define lookup(size_t, c) (((0x0101010101010101*c ^ size_t) - 0x0101010101010101) & ~(0x0101010101010101*c ^ size_t) & 0x8080808080808080)
#define print(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define marker printf("[%s:%d] marker\n", __FILE__, __LINE__);
#define value(x) printf(_Generic((x), int: "[%s:%d] %s: %d\n", char: "[%s:%d] %s: %c\n", uint8_t: "[%s:%d] %s: %c\n", unsigned: "[%s:%d] %s: %u\n", long: "[%s:%d] %s: %ld\n", unsigned long: "[%s:%d] %s: %lu\n", long long: "[%s:%d] %s: %lld\n", unsigned long long: "[%s:%d] %s: %llu\n", float: "[%s:%d] %s: %f\n", double: "[%s:%d] %s: %lf\n", char*: "[%s:%d] %s: %s\n", const char*: "[%s:%d] %s: %s\n", default: "[%s:%d] %s: %p\n"), __FILE__, __LINE__, #x, (x))
#define min(a, b) ((a) > (b) ? (b) : (a))

#endif
