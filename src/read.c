#include "kit.h"

struct file** files;

int file_store(char* filename) {
    FILE* fd = fopen(filename,"r");
    if (!fd) return 1;

    fseek(fd, 0L, SEEK_END);  // Move pointer to the end of the file
    size_t size = ftell(fd);    // Get current position (total bytes)
    fseek(fd, 0L, SEEK_SET);

    char* bytes = auto_free(malloc(size+1));

    size_t got = fread(bytes, sizeof(char), size, fd);
    if (size > got) {
        print("%lu",got);
        return 1;
    }
    bytes[got] = 0;
    
    struct file* file = auto_free(malloc(sizeof(struct file)));
    file->bytes = bytes;    
    file->filelen = size;    
    file->filename = filename;    

    files = array_append(files, file);

    fclose(fd);
    return 0;
}

__attribute__((destructor))
static void cleanup() {
    free(files);
}