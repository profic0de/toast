#include "kit.h"

char** files;
struct block {
    enum {
        KEYWORD,
        STRING,
        NUMBER,
        NEWLINE,
        FUNCTION,
        SYMBOL,
        ROOT
    } type;
    union {
        struct block** blocks;
        char* content;
    };
} root = {.type=FUNCTION};

int parse_fd(FILE* fd) {
    lookup(spaces, " \t\n\r\v\f");
    lookup(separators, " ");
    static unsigned char keywords[32] = {0}; for (int i = 0; i < 128; i++) if (!isalpha(i) && !isdigit(i) && i != '_') bitset(keywords, i); flip(keywords);

    char* bytes = 0;
    char mode = 0;
    char c;
    size_t line = 0;
    size_t column = 0;
    while ((c = getc(fd))!=EOF) {
        if (c=='\n') {column = (++line-line); break;} //This break ruined all the fun :(
        else column++;

        switch (mode) {
        case 0:
            //Reading stuff
            if (bitget(separators,c)) { //Separator detected: determine the block type and add it to the root (by now)
                if (!bytes) break;
                struct block* block = auto_free(calloc(1,sizeof(block)));

                if (isdigit(bytes[0])) {
                    block->type=NUMBER;
                }
                // root.blocks = array_append(root.blocks);
                break;
            }
            str_append(&bytes, c);
            break;
        
        default:
            break;
        }
    }
    // printf("%s\n",bytes?bytes:"");
    free(bytes);
    
    return 0;
}

int file_store(char* filename) {
    struct stat sb;
    if (stat(filename, &sb) == -1) {
        // perror("stat");
        return 1;
    }

    if (!S_ISREG(sb.st_mode)) {
        // print("%s is not a file",filename);
        return 1;
    }

    if (files) {
        char** temp = files-1;
        while (*++temp&&strcmp(*temp,filename));
        if (*temp) return 0;
    }

    FILE* fd = fopen(filename,"r");
    if (!fd) {
        print("failed to open %s",filename);
        return 1;
    }

    fseek(fd, 0L, SEEK_SET);  // Move pointer to the end of the file
    // ftell(fd);
    // char* bytes = auto_free(malloc(size+1));

    // char bytes[4096];
    // size_t got = fread(bytes, sizeof(char), 4096, fd);
    // bytes[got] = 0;
    parse_fd(fd);

    files = array_append(files, auto_free(strdup(filename)));

    char** temp = files;
    while (*temp++);
    // parse_file((int)(temp-files)-2);

    fclose(fd);
    return 0;
}

__attribute__((destructor))
static void cleanup() {
    free(files);
}