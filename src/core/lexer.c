#include "tools/ctype.h"
#include "kit.h"

struct token next_token(char* buffer, char* end, struct project* project) {
again:
    while (is_space((unsigned char)*buffer)) buffer++;
    if (buffer==end) return (struct token){.start=buffer,.type=EOF};
    uint8_t c = *buffer++;

    if (c=='#') {if (end-buffer<8) {while (*buffer&&*buffer++!='\n'); goto again;} else {
        uint64_t val; memcpy(&val, buffer, 8);
        if (val==uint8("require ")) { buffer += 8;
            if (*buffer!='\''&&*buffer!='"'&&*buffer!='<') {while (*buffer&&*buffer++!='\n'); goto again;}

            int load_file(string filepath, struct project* project);

            char end = *buffer=='<'?'>':*buffer, *start = ++buffer;
            while ((*++buffer)!=end&&(*buffer)!='\n');
            uint64_t len = buffer-start;
            if (*buffer=='\n') goto again;
            while (*buffer&&*buffer++!='\n');

            // value(*(start-1));
            if (*(start-1)=='<') { if (project->lib_paths) {
                for (size_t i = 0; project->lib_paths[i] ; i++) {
                    //TODO: Finish this loop
                }
            }} else if (project->src_paths) {
                for (size_t i = 0; project->src_paths[i] ; i++) {
                    //TODO: This one too
                }
            }

            int r; if ((r=load_file((string){.text=start,.len=len}, project))){
                error(project->lf->path.text, start-buffer, len, "error: invalid path");
            }
        } else {while (*buffer&&*buffer++!='\n'); goto again;}
    }}
    print("%c, %c",c ,*buffer);
    if (is_single(c)) return (struct token){.type=SYMBOL,.start=buffer-1};
    if (is_operator(c)) return (struct token){.type=OPERATOR,.start=buffer-1};
    if (is_kws(c)) return (struct token){.type=KEYWORD,.start=buffer-1};
    if (is_digit(c)) return (struct token){.type=NUMBER,.start=buffer-1};

    struct token token;
    token.start=buffer;
    token.type=EOF;

    return token;
}
