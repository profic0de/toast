#include <stdio.h>
#include <stdint.h>
#include "tools/ctype.h"

int main() {
    char bytes[] = "\\\\\"";
    char* buf = bytes+sizeof(bytes)-2;

    int val = (*(buf-1)=='\\'&&*(buf-2)!='\\');

    printf("%c\n",*buf);
    printf("%s: %d\n", bytes, val);

    return 0;
}