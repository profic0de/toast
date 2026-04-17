//This file was first of all made so i won't get annoyed by the asan
#include "kit.h"

static void** pointers;

void* auto_free(void* ptr) {
    if (!ptr) return ptr;
    if (!pointers) {
        pointers = calloc(2,sizeof(void*));
        pointers[0] = ptr;
        return ptr;
    }
    void** temp = pointers; 
    while (*temp++);

    int diff = (int)(temp-pointers);
    pointers[diff-1] = ptr;
    pointers = realloc(pointers, sizeof(void*)*(diff+1));
    pointers[diff] = 0;
    return ptr;
}

__attribute__((destructor))
static void cleanup() {
    void** temp = pointers;
    print("Cleaning...");
    while (*temp) {
        print("%p",*temp);
        free(*temp++);
    }
    free(pointers);   
}