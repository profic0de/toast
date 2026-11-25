#include "kit.h"

static uint8_t is_type[256];
__attribute__((constructor))
static void start() {
    uint8_t types[] = {
        type, kw_func, kw_let,
        kw_var, type_ident
    };
    size_t i = sizeof(types);
    while (i--) is_type[types[i]]++;
}

// TODO: Make this lex the types (func, u8, u8[,,], func[], ref func[], ) like it does in decl.c
tok_t* lex_type(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t val = {0}, *tok; file_t* file;
    if (!(file=(val.file=file_top(proj)))) return NULL;
    val.line=file->contents.newlines;
    val.line_start=file->contents.line_start;
    val.start=file->contents.ptr;
    tok_t* allocated_tok = get_tok(proj);
    if (allocated_tok) tok=allocated_tok;
    else tok = lex_ident(proj);
    if (!tok||!is_type[tok->type]) {
        if (allocated_tok) return (set_tok(proj), NULL);
        return (free(tok), NULL);
    }
    log("type");

    set_tok(proj);
    return rtok(&val);
}
