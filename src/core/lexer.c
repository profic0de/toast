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
                error(project->lf->path.text, token_start-start, len, "error: invalid path");
                return (struct token){.type=EOF};
            }
            goto again;
        } else {while (*buffer&&*buffer++!='\n'); goto again;}
    }}
    if (is_single(c)) return (struct token){.type=SYMBOL,.start=buffer-1};
    if (is_operator(c)) return (struct token){.type=OPERATOR,.start=buffer-1};
    if (is_kws(c)) return (struct token){.type=KEYWORD,.start=buffer-1};
    if (is_digit(c)) return (struct token){.type=NUMBER,.start=buffer-1};

    return (struct token){.type=EOF};
}

struct token next_token(char** buffer, char* start, char* end, struct project* project) {
    // enum token_type {EOF,NUMBER,FLOAT,KEYWORD,SYMBOL,STRING,PATH,WORD,OPERATOR};
    struct file* file = project->lf;
    struct token token = next_token_start(*buffer, start, end, project);

    switch (token.type) {
    case SYMBOL: {
        *buffer = token.start+1;
        return (struct token){.type=SYMBOL,.start=token.start,.len=1};
    }
    
    case OPERATOR: {
        uint8_t i = 0;
        uint8_t buf[5] = {0};

        *buffer = token.start;

        memcpy(buf, *buffer, min(sizeof(buf)-1, end-(*buffer)));
        i=0; while (++i) if (!is_operator(buf[i])) {
            memset(buf+i, 0, sizeof(buf)-1-i);
            break;
        } *buffer += i;
        if (buf[3]) {error(file->path.text, token.start-start, i, "error: invalid operator"); goto exit;}

        uint32_t oper = *((uint32_t*)buf);
        uint32_t* opers = are_operators-1;

        while (*++opers) if (*opers==oper) return (struct token){.start=token.start, .len=i, .type=OPERATOR};

        error(file->path.text, token.start-start, i, "error: invalid operator");
        break;
    }

    case KEYWORD: {
        *buffer = token.start;
        uint8_t* buf = (uint8_t*)*buffer;
        while (is_kw(*++buf));

        size_t len = buf-(uint8_t*)*buffer;
        *buffer += len;
        return (struct token){.start=token.start, .len=len, .type=KEYWORD};

        break;
    }

    case NUMBER: {
        *buffer = token.start;
        uint8_t* buf = (uint8_t*)*buffer;

        while (is_digit(*buf))
        break;
    }

    case STRING: {

        break;
    }

    default: break;
    }

exit:
    return (struct token){.type=EOF};
}
