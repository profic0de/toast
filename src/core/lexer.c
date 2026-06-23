#include "tools/ctype.h"
#include "kit.h"

static inline struct token next_token_start(char* buffer, char* start, char* end, struct project* project) {
again:
    while (is_space((unsigned char)*buffer)) buffer++;
    if (buffer==end) return (struct token){.start=buffer,.type=EOF};
    uint8_t c = *buffer++;

    if (c=='#') {if (end-buffer<8) {while (*buffer&&*buffer++!='\n'); goto again;} else {
        uint64_t val; memcpy(&val, buffer, 8);
        if (val==uint8("require ")) { buffer += 8;
            if (*buffer!='\''&&*buffer!='"'&&*buffer!='<') {while (*buffer&&*buffer++!='\n'); goto again;}
            
            int load_file(string filepath, struct project* project);

            char end = *buffer=='<'?'>':*buffer, *token_start = ++buffer;
            while ((*++buffer)!=end&&(*buffer)!='\n');
            uint64_t len = buffer-token_start;
            if (*buffer=='\n') goto again;
            while (*buffer&&*buffer++!='\n');

            // value(*(start-1));
            if (*(token_start-1)=='<') { if (project->lib_paths) {
                for (size_t i = 0; project->lib_paths[i] ; i++) {
                    //TODO: Finish this loop
                }
            }} else if (project->src_paths) {
                for (size_t i = 0; project->src_paths[i] ; i++) {
                    //TODO: This one too
                }
            }

            int r; if ((r=load_file((string){.text=token_start,.len=len}, project))){
                error(project->lf->path.text, token_start-start, len+1, "error: invalid path");
                return (struct token){.type=ERR};
            }
            goto again;
        } else {while (*buffer&&*buffer++!='\n'); goto again;}
    }}
    if (is_single(c)) return (struct token){.type=SYMBOL,.start=buffer-1};
    if (is_operator(c)) return (struct token){.type=OPERATOR,.start=buffer-1};
    if (is_kws(c)) return (struct token){.type=KEYWORD,.start=buffer-1};
    if (is_digit(c)) return (struct token){.type=NUMBER,.start=buffer-1};
    if (c=='"') return (struct token){.type=STRING,.start=buffer-1};
    if (c=='\'') return (struct token){.type=NSTRING,.start=buffer-1};

    return (struct token){.type=EOF};
}

struct token next_token(char** buffer, char* start, char* end, struct project* project) {
    // enum token_type {EOF,NUMBER,FLOAT,KEYWORD,SYMBOL,STRING,PATH,WORD,OPERATOR};
    struct file* file = project->lf;
    struct token token = next_token_start(*buffer, start, end, project);
    *buffer = token.start;

    switch (token.type) {
    case SYMBOL: {
        *buffer += 1;
        return (struct token){.type=SYMBOL,.start=token.start,.len=1};
    }
    
    case OPERATOR: {
        uint8_t i = 0;
        uint8_t buf[5] = {0};

        memcpy(buf, *buffer, min(sizeof(buf)-1, end-(*buffer)));
        i=0; while (++i) if (!is_operator(buf[i])) {
            memset(buf+i, 0, sizeof(buf)-1-i);
            break;
        } *buffer += i;

        if (buf[3]) {
            error(file->path.text, token.start-start, i, "error: invalid operator");
            return (struct token){.type=ERR};
        }

        uint32_t oper = *((uint32_t*)buf);
        uint32_t* opers = are_operators-1;

        while (*++opers) if (*opers==oper) return (struct token){.start=token.start, .len=i, .type=OPERATOR};

        error(file->path.text, token.start-start, i, "error: invalid operator");
        return (struct token){.type=ERR};
    }

    case KEYWORD: {
        uint8_t* buf = (uint8_t*)*buffer;
        while (is_kw(*++buf));

        size_t len = buf-(uint8_t*)*buffer;
        *buffer += len;
        return (struct token){.start=token.start, .len=len, .type=KEYWORD};
    }

    case NUMBER: {
        uint8_t* buf = (uint8_t*)*buffer;
        token.i += (*buf-'0')*10;
    again_n:
        while (is_digit(*++buf)) token.f = token.f*10+*buf-'0';
    
        if (*buf=='_') goto again_n;
        size_t len = buf-(uint8_t*)token.start;
        if (*buf=='.') (token.type=FLOAT)&&(token.f==(double)token.i)&&(token.len=len);
        else {
            *buffer += len;
            return (struct token){.start=token.start, .len=len, .type=NUMBER, .i=token.i};
        }
        double s = 1;

    again_f:
        while (is_digit(*++buf)) token.f += (s/=10)*(*buf-'0');
        if (*buf=='_') goto again_f;
        len = buf-(uint8_t*)token.start;
        *buffer += len;
        return (struct token){.start=token.start, .len=len, .type=FLOAT, .f=token.f};
    }

    case STRING: {
        uint8_t* buf = (uint8_t*)*buffer;
        if (*(buf+1)=='"'&&*(buf+2)=='"') {
            buf+=2;
    again_bs:
            while (*++buf!='"'&&*buf);
            if (!buf[0]) {
                error(file->path.text, token.start-start, 3, "error: string was never closed");
                return (struct token){.type=ERR};
            }

            if (*(buf+1)!='"'||*(buf+2)!='"') goto again_bs;
            if ((*(buf-1)=='\\'&&*(buf-2)!='\\')) goto again_bs;

            size_t len = buf-(uint8_t*)token.start;
            *buffer += len+3;
            return (struct token){.start=token.start+3, .len=len-3, .type=BSTRING};

        } else {
    again_s:
            while (*++buf!='"'&&*buf&&*buf!='\n');
            if (!buf[0]||*buf=='\n') {
                error(file->path.text, token.start-start, 1, "error: string was never closed");
                return (struct token){.type=ERR};
            }
            if ((*(buf-1)=='\\'&&*(buf-2)!='\\')) goto again_s;
            
            size_t len = buf-(uint8_t*)token.start;
            *buffer += len+1;
            return (struct token){.start=token.start+1, .len=len-1, .type=STRING};
        }
    }

    case NSTRING: {
        uint8_t* buf = (uint8_t*)*buffer;
    again_ns:
        while (*++buf!='\''&&*buf&&*buf!='\n');
        if (!buf[0]||*buf=='\n') {
            error(file->path.text, token.start-start, 1, "error: string was never closed");
            return (struct token){.type=ERR};
        }
        if ((*(buf-1)=='\\'&&*(buf-2)!='\\')) goto again_ns;
        size_t len = buf-(uint8_t*)token.start-1;
        if (len>8) {
            error(file->path.text, token.start-start, 1, "error: numeric string was too long");
            return (struct token){.type=ERR};
        }
        memcpy(&token.i, token.start+1, len);
        *buffer+=len+2;
        return (struct token){.start=token.start+1, .len=len, .type=NUMBER, .i=token.i};
    }

    default: break;
    }

    return (struct token){.type=EOF};
}
