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
    size_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char* buffer = malloc(size), *ptr = buffer-1, *end = ptr; {
        size_t len = read(fd, buffer, size);
        if (0 >= len) return 2;
        end += len;
    }

    struct token token;

    while ((token = next_token(buffer, end)).type) {
        if (token.buffer)
            print("type: %s, token: %.*s",type_to_char(token.type),(int)token.len,token.buffer);
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

int file_store(char* filename) {
    struct stat sb;
    if (stat(filename, &sb) == -1) {
        perror("stat");
        return 1;
    }

    if (!S_ISREG(sb.st_mode)) {
        print("%s is not a file",filename);
        return 2;
    }

    if (files) {
        struct file** temp = files-1;
        while (*++temp&&strcmp(temp[0]->filename,filename));
        if (*temp) return 0;
    }

    size_t fd = open(filename, O_RDONLY);
    if (!fd) {
        print("failed to open %s",filename);
        return 3;
    }

    struct file* file = auto_free(malloc(sizeof(struct file)));
    file->filename = auto_free(strdup(filename));
    file->requirements = NULL;

    files = array_append(files, file);

    int r = parse_file(fd);

    auto_free(file->requirements);

    close(fd);
    return r;
}