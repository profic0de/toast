#include "kit.h"

int file_store(char* filename);

int main(int argc, char *argv[]) {
    return file_store(argv[argc-1]);
}