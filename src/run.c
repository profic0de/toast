#include "kit.h"

struct scope {
    
} root;

int run_file(int idx) {
    struct token** temp = files[idx]->tokens-1;
    while (*++temp) {
        printf("%d:%s\n",temp[0]->type,temp[0]->value);
    }
    return 0;
}