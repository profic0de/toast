#include "kit.h"

int main() {
    size_t fd = open("test.c", O_RDONLY);
    size_t size = lseek(fd, 0, SEEK_END);

    printf("%lu\n", size);

    return 0;
}