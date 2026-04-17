// I really like null terminated arrays
#include "kit.h"

void** array_add(void** arr, void* ptr) {
    if (!ptr) return arr;
    if (!arr) {
        void** temp = calloc(2, sizeof(ptr));
        temp[0] = ptr;
        return temp;
    }
    void** temp = arr; 
    while (*temp++);

    int diff = (int)(temp-arr);
    arr[diff-1] = ptr;
    arr = realloc(arr, sizeof(void*)*(diff+1));
    arr[diff] = 0;
    return arr;
}