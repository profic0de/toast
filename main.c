#include "kit.h"

int main(int argslen, char** args) {
    if (args[1][0]) open_file(args[1]);
    
    return 0;
}