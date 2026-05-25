#include "kit.h"

int main() {
    size_t fd = open("test.c", O_RDONLY);

    char c = '\0';
    if (read(fd, &c, 1)!=1) return 1;

    size_t size = lseek(fd, 0, SEEK_END);

    printf("%c,%lu\n", c,size);

    return 0;
}