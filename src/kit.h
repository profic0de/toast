#ifndef KIT_H
#define KIT_H

#define _GNU_SOURCE
#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <sys/ioctl.h>

#include "structs.h"

#define log(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define debug(x) _Generic((x), short: log(#x ": %hd", (x)), unsigned short: log(#x ": %hu", (x)), int: log(#x ": %d", (x)), unsigned int: log(#x ": %u", (x)), long: log(#x ": %ld", (x)), unsigned long: log(#x ": %lu", (x)), long long: log(#x ": %lld", (x)), unsigned long long: log(#x ": %llu", (x)), float: log(#x ": %f", (x)), double: log(#x ": %lf", (x)), char: log(#x ": '%c'", (x)), char *: log(#x ": \"%s\"", (x)), const char *: log(#x ": \"%s\"", (x)), void *: log(#x ": %p", (x)), void **: log(#x ": %p", (x)) )
#define u64(x) ((uint64_t)(x))&0xFF, (((uint64_t)(x))>>8)&0xFF, (((uint64_t)(x))>>16)&0xFF, (((uint64_t)(x))>>24)&0xFF, (((uint64_t)(x))>>32)&0xFF, (((uint64_t)(x))>>40)&0xFF, (((uint64_t)(x))>>48)&0xFF, (((uint64_t)(x))>>56)&0xFF
#define u32(x) (x)&0xFF, ((x)>>8)&0xFF, ((x)>>16)&0xFF, ((x)>>24)&0xFF
#define s8(b) ((uint64_t)(b "\0\0\0\0\0\0\0\0")[0]|(uint64_t)(b "\0\0\0\0\0\0\0\0")[1]<<8|(uint64_t)(b "\0\0\0\0\0\0\0\0")[2]<<16|(uint64_t)(b "\0\0\0\0\0\0\0\0")[3]<<24|(uint64_t)(b "\0\0\0\0\0\0\0\0")[4]<<32|(uint64_t)(b "\0\0\0\0\0\0\0\0")[5]<<40|(uint64_t)(b "\0\0\0\0\0\0\0\0")[6]<<48|(uint64_t)(b "\0\0\0\0\0\0\0\0")[7]<<56)
#define min(a, b) (a>b?b:a)

size_t index_append(void*** arr, void* ptr);
size_t index_remove(void*** arr, size_t index);
size_t index_ptr(void*** arr, void* ptr);
size_t bytes_append(uint8_t** dest, uint8_t* src, size_t n);
void* allocpy(void* val, size_t size);
void* index_pop(void*** arr);
void* index_top(void*** arr);
void print_bits(uint64_t v);

file_t* file_top(proj_t* proj);
file_t* open_file(proj_t* proj, char* path);
tok_t* lex_statement(proj_t* proj);
bool lex_spaces(proj_t* proj);
tok_t* lex_ident(proj_t* proj);
void* lexer_error(proj_t* proj, char* msg);
tok_t* rtok(proj_t* proj, tok_t* tok);
tok_t* rlcl(proj_t* proj, tok_t* tok);

#endif