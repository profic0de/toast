#include "kit.h"

char* type_to_char(enum token_type token_type) {
    static char str[16];
    for (size_t i = 0; i < sizeof(str); i++) str[i] = 0;

    switch (token_type) {
    #define case_macro(val) case val: return (strcpy(str, #val), str);

    case_macro(NONE);
    case_macro(NUMBER);
    case_macro(FLOAT);
    case_macro(KEYWORD);
    case_macro(SYMBOL);
    case_macro(STRING);
    case_macro(PATH);
    case_macro(WORD);

    #undef case_macro
    default:
        return (strcpy(str, "[ ]"), str);
    }
    return str;
}

struct token next_token(char* buffer, char* end);

int parse_file(size_t fd) {
    print("test");
    size_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char* buffer = malloc(size+1), *ptr = buffer, *end = ptr; {
        size_t len = read(fd, buffer, size);
        if (0 >= len) return 1;
        end += len;
    } buffer[size] = 0;

    struct token token;

    while ((token = next_token(ptr, end)).type) {
        if (token.buffer)
            print("type: %s, token: %.*s",type_to_char(token.type),(int)token.len,token.buffer);
        ptr = token.buffer+token.len;
    }

    free(buffer);

    // if (bytes) {
    //     tokens = array_append(tokens, strdup(*bytes));
    //     types = array_append(types, token_type);
    //     return NULL;
    // }

    // size_t len = 0; 
    // for (size_t i=0; tokens[i]; len=++i) types[i] = types[i*2]; // Shifting the types array bc the enum is int and the array consists of pointers so i need to multiply the index by 2 to skip the 0's :)

    // printf("declared %s, (%s)\n",tokens[i+1], tokens[i]);

    return 0;
}
