#include "kit.h"

int file_store(char* filename) {
    FILE* file = fopen(filename,"r");
    if (!file) return 1;

    fseek(file, 0L, SEEK_END);  // Move pointer to the end of the file
    size_t size = ftell(file);    // Get current position (total bytes)
    fseek(file, 0L, SEEK_SET);

    char* bytes = malloc(size+1);

    size_t got = fread(bytes, sizeof(char), size, file);
    if (size > got) {
        free(bytes);
        print("%lu",got);
        return 1;
    }
    bytes[got] = 0;

    // strtok();

    free(bytes);
    return 0;
}