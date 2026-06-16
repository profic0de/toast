#include <stdio.h>
#include <stdint.h>
#include "tools/ctype.h"

int main() {
    char bytes[] = "\\\"";
    char* buf = bytes+sizeof(bytes)-1;

    int val = (*(buf-2)=='\\'&&*(buf-3)!='\\');

    printf("%d\n",val);

    return 0;
}