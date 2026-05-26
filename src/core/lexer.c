#include "kit.h"

#define POS ptr-buffer

int next_token(char* buffer, char* end) {
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

            size_t start = POS;
            size_t pos = POS;

            size_t val = c;
            size_t len = 1;

            if (lookup(*(size_t*)"[]{}();,", c) || c=='.' || c=='~') {
                ptr++;
            } else {
                while (chr && operators[c] && !((val = (val << 8) | c) & 0xFF000000UL)) {
                    len++;
                }
            }

            if (val & 0xFF000000UL)
                return (
                    FREE,
                    error(file->filename, pos, 3, "error: invalid symbol"),
                    ERROR(12)
                );

            bytes = strndup(buffer + start, len);
            ptr--;
        } else if (c=='\''||c=='"') {
            token_type = STRING;
            size_t start = POS, len = 0;
            char b = c, p = 0, po = 0;
            while (chr && !(c==b && !(p=='\\' && po!='\\')) && c!='\n') po = (len++, p), p = c;
            bytes = strndup(buffer+start,len);
            // print("%c",*++ptr);
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

__attribute__((destructor))
static void clean() {
    free(files);
}