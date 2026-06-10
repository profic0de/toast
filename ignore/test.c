#include "kit.h"
char str_box[UINT16_MAX+1];

int main() {
    if (!realpath("../", str_box+1)) return 1;
    int len = strlen(str_box+1);
    char* filename = str_box+len+1;
    while (*--filename&&*filename!='/'); filename++;
    int fl = str_box+len-filename+1;

    value(filename);
    value(fl);
    value(len);

    return 0;
}