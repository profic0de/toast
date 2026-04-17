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
    
    print("dif: %d",(int)(temp-pointers));
    // pointers;

}