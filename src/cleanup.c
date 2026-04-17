//This file was first of all made so i won't get annoyed by the asan
#include "kit.h"

static void** pointers;

void auto_free(void* ptr) {
    if (!ptr) return;
    if (!pointers) {
        pointers = calloc(2,sizeof(void*));
        pointers[0] = ptr;
        return;
    }
    void** temp = pointers; 
    while (*temp++);
    
    int diff = (int)(temp-pointers);
    print("dif: %d",diff);
    pointers[diff-1] = ptr;
    pointers = realloc(pointers, sizeof(void*)*(diff+1));
    pointers[diff] = 0;
}

__attribute__((destructor))
static void cleanup() {
    void** temp = pointers;
    while (*temp) {
        print("%p",*temp);
        free(*temp++);
    }
    free(pointers);   
}