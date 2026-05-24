#include "kit.h"

int main() {
    FILE* f = fopen("test.c","r");

    printf("%c\n",fgetc(f));

    return 0;
}