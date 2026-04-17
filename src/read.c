#include "kit.h"

struct file** files;

int file_store(char* filename) {
    FILE* file = fopen(filename,"r");
    if (!file) return 1;

    fseek(file, 0L, SEEK_END);  // Move pointer to the end of the file
    size_t size = ftell(file);    // Get current position (total bytes)
    fseek(file, 0L, SEEK_SET);

    char* bytes = auto_free(malloc(size+1));

    size_t got = fread(bytes, sizeof(char), size, file);
    if (size > got) {
        print("%lu",got);
        return 1;
    }
    bytes[got] = 0;
    
    if (!files) {
        files = calloc(2,sizeof(void*));
    }
    
    return 0;
}

__attribute__((destructor))
static void cleanup() {
    free(files);
}