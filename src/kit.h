#ifndef KIT_H
#define KIT_H

// open, read, etc
#include <fcntl.h>
#include <unistd.h>
// #include <stdio.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stdint.h>

extern struct file {
    char* filename;
    char** requirements;
}** files;

void* auto_free(void* ptr);
void str_append(char** str, char c);
int dict_append(char*** arr, char* ptr);
void** array_append(void** arr, void* ptr);
void error(const char* filename, size_t pos, size_t token_len, const char* fmt, ...);

#undef EOF
enum token_type {EOF,NUMBER,FLOAT,KEYWORD,SYMBOL,STRING,PATH,WORD,REQ};
struct token {
    enum token_type type;
    char* buffer;
    size_t len;
};

#define lookup(size_t, c) (((0x0101010101010101*c ^ size_t) - 0x0101010101010101) & ~(0x0101010101010101*c ^ size_t) & 0x8080808080808080)
#define array_append(arr, ptr) ((__typeof__(arr))array_append(((void**)(arr)), ((void*)(ptr))))
#define print(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif
