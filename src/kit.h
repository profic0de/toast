#ifndef KIT_H
#define KIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

void auto_free(void* ptr);
#define print(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
extern struct file {
    char* filename;
    size_t filelen;
    char* bytes;
}** files;

#endif