#include "kit.h"

size_t index_append(void*** arr, void* ptr) {
    if (!arr||!ptr) return 0;
    #define arr (*arr)
    if (!arr) {
        arr = calloc(1, sizeof(ptr)*3);
        // arr[0] = max_index (practically role of capacity)
        // arr[1] = min_empty_index
        arr[1] = (void*)1;
        arr[2] = ptr;
        return 0;
    }
    size_t im = (size_t)arr[0], id = (size_t)arr[1];
    if (id>im) {
        arr = realloc(arr, sizeof(ptr)*(id+3));
        arr[id+2] = ptr;
        arr[0] = (void*)id, arr[1]++;
        return id;
    } for (size_t i=2; i<=im+2; i++) if (!arr[i]) {
        arr[i] = ptr;
        arr[1]++;
        return i-2;
    }
    return 0;
}

#undef arr
size_t index_remove(void*** arr, size_t index) {
    if (!arr||!*arr) return 0;
    #define arr (*arr)
    if (index>(size_t)arr[0]) return 0;
    arr[index+2] = NULL;
    if ((size_t)arr[1]>index) arr[1] = (void*)index;
    return index;
}

#undef arr
void* index_pop(void*** arr) {
    if (!arr||!*arr) return NULL;
    #define arr (*arr)
    size_t top = (size_t)arr[1];
    if (!top--) return NULL;
    void* ptr = arr[top+2];
    arr[top+2] = NULL;
    arr[1] = (void*)top;
    return ptr;
}

#undef arr
void* index_top(void*** arr) {
    if (!arr||!*arr) return NULL;
    #define arr (*arr)
    size_t top = (size_t)arr[1];
    if (!top--) return NULL;
    return arr[top+2];
}

#undef arr
size_t index_ptr(void*** arr, void* ptr) {
    if (!arr||!*arr) return 0;
    #define arr (*arr)
    for (size_t i=2; i<=(size_t)arr[0]+2; i++) if (arr[i]==ptr) {
        arr[i] = NULL;
        if ((size_t)arr[1]>i-2) arr[1] = (void*)i-2;
        return 1;
    }
    return 0;
}

#undef arr
size_t bytes_append(uint8_t** dest, uint8_t* src, size_t n) {
    if (!dest) return 0;
    #define arr (*dest)
    if (!n||!src) return arr?((size_t*)arr)[-1]:0;
    if (!arr) {
        arr = malloc(8+n)+8;
        memcpy(arr, src, n);
        return (((size_t*)arr)[-1] = n);
    }
    size_t s = ((size_t*)arr)[-1];
    arr = realloc(arr-8, 8+n+s)+8;
    memcpy(arr+s, src, n);
    ((size_t*)arr)[-1] = n+s;    
    return n+s;
}

#undef arr

void* allocpy(void* val, size_t size) {
    if (!val||!size) return NULL;
    void* ptr = malloc(size);
    if (!ptr) return NULL;
    memcpy(ptr, val, size);
    return ptr;
}

#if 0
__attribute__((constructor))
static void test() {
    uint8_t str[] = "123";
    uint8_t* arr;
    DEBUG(bytes_append(&arr, str, sizeof(str)-1));
    DEBUG(bytes_append(&arr, str, sizeof(str)-1));
    DEBUG(bytes_append(&arr, 0, 0));
}
#endif