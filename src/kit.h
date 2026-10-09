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

#define LOG(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG(x) _Generic((x), short: LOG(#x ": %hd", (x)), unsigned short: LOG(#x ": %hu", (x)), int: LOG(#x ": %d", (x)), unsigned int: LOG(#x ": %u", (x)), long: LOG(#x ": %ld", (x)), unsigned long: LOG(#x ": %lu", (x)), long long: LOG(#x ": %lld", (x)), unsigned long long: LOG(#x ": %llu", (x)), float: LOG(#x ": %f", (x)), double: LOG(#x ": %lf", (x)), char: LOG(#x ": '%c'", (x)), char *: LOG(#x ": \"%s\"", (x)), const char *: LOG(#x ": \"%s\"", (x)), void *: LOG(#x ": %p", (x)), void **: LOG(#x ": %p", (x)) )
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

#endif