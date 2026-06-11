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

struct file {
    string name;
    string path;
};

struct project {
    struct file* main_file;

    char** src_paths;
    char** lib_paths;
    struct hashmap* files;
    struct file* lf; // last file
};

void* auto_free(void* ptr);
void str_append(char** str, char c);
int dict_append(char*** arr, char* ptr);
void** array_append(void** arr, void* ptr);
void error(const char* filename, size_t pos, size_t token_len, const char* fmt, ...);

#undef EOF
enum token_type {EOF,NUMBER,FLOAT,KEYWORD,SYMBOL,STRING,PATH,WORD,REQ,SKIP,ERR};
struct token {
    enum token_type type;
    char* buffer;
    uint64_t len;
};

#define uint8(s) (__extension__ ({ uint64_t _v; __builtin_memcpy(&_v, s, 8); _v; }))
#define lookup(size_t, c) (((0x0101010101010101*c ^ size_t) - 0x0101010101010101) & ~(0x0101010101010101*c ^ size_t) & 0x8080808080808080)
#define array_append(arr, ptr) ((__typeof__(arr))array_append(((void**)(arr)), ((void*)(ptr))))
#define print(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define value(x) printf(_Generic((x), int: "[%s:%d] %s: %d\n", char: "[%s:%d] %s: %c\n", unsigned: "[%s:%d] %s: %u\n", long: "[%s:%d] %s: %ld\n", unsigned long: "[%s:%d] %s: %lu\n", long long: "[%s:%d] %s: %lld\n", unsigned long long: "[%s:%d] %s: %llu\n", float: "[%s:%d] %s: %f\n", double: "[%s:%d] %s: %lf\n", char*: "[%s:%d] %s: %s\n", const char*: "[%s:%d] %s: %s\n", default: "[%s:%d] %s: %p\n"), __FILE__, __LINE__, #x, (x))
#define min(a, b) ((a) > (b) ? (b) : (a))

#endif
