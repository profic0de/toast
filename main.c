#include "kit.h"

proj_t proj;

int main(int argslen, char** args) {
    if (args[1][0]) open_file(&proj, args[1]);
    if (args[1][0]) open_file(&proj, args[1]);
    
    return 0;
}