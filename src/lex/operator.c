#include "kit.h"

static uint8_t single[256];
__attribute__((constructor))
static void start() {
    uint8_t var_types[] = {
        ';', '(', ')', '[', ']', '{', '}',
    };
    size_t i = sizeof(var_types);
    while (i--) single[var_types[i]] = sizeof(var_types)-i;
}

tok_t* lex_operator(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t tok = {0};
    file_t* file = (tok.file = file_top(proj));
    if (!file) return NULL;
    tok.start      = file->contents.ptr;
    tok.line       = file->contents.newlines;
    tok.line_start = file->contents.line_start;
    tok.type       = oper;
    tok.len        = 1;

    uint8_t* ptr = (uint8_t*)tok.start; // ptr[0] can't be '\0' because of lex_spaces
    size_t len = 1;
    if (single[ptr[0]]) tok.type+=single[ptr[0]];
    else if (ptr[0]=='+') {
        // +++++...           which means that             +++
        // ^^ ^^ - should be increment and stuff            ^^ - increment and stuff
        //   ^   - addition sign                           ^   - addition sign
        //
        // ++++ - would cause an error because you can't split it properly
        while ((++ptr)[0]=='+') if (++len>=4) break;
        if (len==1||len==3) tok.type = oper_addition;
        else tok.len += !!(tok.type=oper_incr);
    } else if (ptr[0]=='-') {
        // same story as with +
        while ((++ptr)[0]=='-') if (++len>=4) break;
        if (len==1||len==3) tok.type = oper_substraction;
        else tok.len += !!(tok.type=oper_decr);
    } else if (ptr[0]=='=') {
        while ((++ptr)[0]=='=') if (++len>=2) break;
        if (len==1) tok.type = oper_assign;
        else tok.len += !!(tok.type=oper_equals);
    }
    else return NULL;

    file->contents.ptr += tok.len;
    return rlcl(proj, &tok);
}