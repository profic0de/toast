#include "kit.h"
char str_box[UINT16_MAX+1];

int main() {
    char* path = realpath("../", str_box);
    print("%s", path);

    return 0;
}