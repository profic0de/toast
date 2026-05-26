#include "kit.h"

struct file** files;

#define stack_block(type, ptr) root.blocks = array_append(root.blocks, make_block(type, ptr))
#define FREE (free(bytes),free(buffer))
#define ERROR(i) 100+i
#define POS ptr-buffer

int parse_fd(size_t fd) {
    size_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char* buffer = malloc(size), *end = buffer, *ptr = buffer-1; {
        size_t len = read(fd, buffer, size);
        if (0 >= len) return 2;
        end += len;
    }

    static char* list = "+-/*!=%%><()[]{}&|~^;,.";
    static char operators[256];
    if (operators[(int)*list]) goto skip;
    char* _list = list-1;
    while (*++_list) operators[(int)*_list]++;
    skip:

    struct file** f = files-1; while (*++f); f -= 1;
    struct file* file = f[0];
    #define chr ((++ptr)!=end&&(c=*ptr))

    int c = 0;
    char* bytes = NULL;
    while (ptr!=end) {
        enum token_type token_type = NONE;

        if (!chr) break;
        if (isspace(c)) continue;
        if (c=='#') {
            size_t val = 0, pos = 0, len = 0;
            while (chr&&c!='\n') if (!(val&((size_t)0xFF<<(8*7)))) val = (val<<8)|c; else pos += (len++, (!pos)*(POS));
            bytes = strndup(buffer+pos,len);
            if (val==((size_t)'requ'<<(8*4)|'ire ')&&((c=bytes?bytes[0]:0)=='\''||c=='"'||c=='<')) {
                char* b = bytes+1;
                char e = bytes[0]=='<'?'>':bytes[0];

                while (b[0]&&b[0]!=e) b++;

                // TODO: Implement requirement loading (really easy)
                // printf("This module requires: %.*s\n", (int)(b-bytes-1), bytes+1);
            }
            bytes = (free(bytes), NULL);
            continue;
        }

        if (isdigit(c)) {
            token_type = NUMBER;
            size_t start = POS, len = 1;
            while (chr) {
                size_t pos = POS;
                if (isspace(c)||(operators[c]&&c!='.')) {ptr--;break;}
                else if (isalpha(c)) return (FREE,error(file->filename, pos, 1, "error: a number can't contain letters"), ERROR(10));
                else if (c=='.'&&token_type==NUMBER) token_type = FLOAT;
                else if (c=='.') return (FREE,error(file->filename, pos, 1, "error: a number can only have one dot"), ERROR(11));
                len++;
            }
            bytes = strndup(buffer+start,len);
        } else if (operators[c]) {
            token_type = SYMBOL;
            str_append(&bytes, c);
            size_t val = c, pos = POS;
            if (lookup(*(size_t*)"[]{}();,", c)||c=='.'||c=='~') ptr++;
            else while (chr&&operators[c]&&!((val=(val<<8)|c)&0xFF000000UL)) str_append(&bytes, c);
            if (val&0xFF000000UL) return (FREE, error(file->filename, pos, 3, "error: invalid symbol"), ERROR(12));
            ptr--;
        } else if (c=='\''||c=='"') {
            token_type = STRING;
            size_t start = POS, len = 0;
            char b = c, p = 0, po = 0;
            while (chr
                &&c==b?(po!=p&&p=='\\'):1&&c!='\n') po = (len++, p), p = c;
            bytes = strndup(buffer+start,len);
            if (c!=b) return (FREE, error(file->filename, start, 1, "error: string not closed"), ERROR(13));

        } else {
            token_type = KEYWORD;
            char p = 0;
            size_t pos = POS, val = c, len = 1;
            size_t start = pos;
            while (chr) {
                if (!(isalnum(c)||c=='.')) {
                    if (p=='.') {
                        if (c=='\n') ptr--;
                        return (FREE, error(file->filename, pos, 1, "error: expected a keyword"), ERROR(14));
                    } break;
                } if (p=='.'&&!isalpha(c)) return (FREE, error(file->filename, pos+1, 1, "error: invalid keyword"), ERROR(15));
                if (c=='.') token_type = PATH;
                if (p==c&&c=='.') return (FREE, error(file->filename, pos, 1, "error: expected a keyword"), ERROR(16));
                p=c;
                pos = POS;
                val = (val<<8)|c;
                len++;
            }
            bytes = strndup(buffer+start,len);

            if (token_type==KEYWORD&&len<=7) {
                // break, if, while, else, return
                size_t words[] = {
                    (size_t)'brea'<<(8*1)|'k',
                    (size_t)'if',
                    (size_t)'whil'<<(8*1)|'e',
                    (size_t)'else',
                    (size_t)'retu'<<(8*2)|'rn',
                    (size_t)'cont'<<(8*4)|'inue',
                    (size_t)'obj',
                    0
                };
                
                int i = 0;
                while (words[i]) if (words[i++]==val) token_type = WORD;

            }
            ptr--;
        }

        handle_token(&bytes, token_type);
        if (bytes) bytes = (free(bytes), NULL);
    }
    handle_token(NULL, 0);
    free(buffer);

    return 0;
}

int file_store(char* filename) {
    struct stat sb;
    if (stat(filename, &sb) == -1) {
        // perror("stat");
        return ERROR(20);
    }

    if (!S_ISREG(sb.st_mode)) {
        // print("%s is not a file",filename);
        return ERROR(21);
    }

    if (files) {
        struct file** temp = files-1;
        while (*++temp&&strcmp(temp[0]->filename,filename));
        if (*temp) return 0;
    }

    size_t fd = open(filename, O_RDONLY);
    if (!fd) {
        print("failed to open %s",filename);
        return ERROR(22);
    }

    struct file* file = auto_free(malloc(sizeof(struct file)));
    file->filename = auto_free(strdup(filename));
    file->requirements = NULL;

    files = array_append(files, file);

    int r = parse_fd(fd);

    auto_free(file->requirements);

    close(fd);
    return r;
}

__attribute__((destructor))
static void clean() {
    free(files);
}