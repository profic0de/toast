#include "kit.h"

static size_t kws[] = {
    s8("func"), s8("return"),
    s8("var"), s8("let"), s8("type"),
    s8("solid"), s8("sol"),
};

tok_t* lex_ident(proj_t* proj) {
    if (!lex_spaces(proj)) return NULL;
    tok_t tok = {0};
    file_t* file = (tok.file = file_top(proj));
    if (!file) return NULL;
    tok.start      = file->contents.ptr;
    tok.line       = file->contents.newlines;
    tok.line_start = file->contents.line_start;
    tok.type       = ident;
    if (!isalpha(file->contents.ptr[0])) return NULL;
    while (isalnum(*++file->contents.ptr)||file->contents.ptr[0]=='_');
    tok.len        = file->contents.ptr-tok.start;

    size_t i = sizeof(kws)/8, t=0;
    if (tok.len<=8) {
        memcpy(&t, tok.start, tok.len);
        while (i--) if (kws[i]==t) i=!(tok.type=kw+i+1);
        // debug(tok.type);
        if (tok.type==kw_sol) tok.type=kw_solid;
    }
    if (tok.type!=ident) return rtok(proj, &tok);
    if (file->locals) {
        size_t i = (size_t)file->locals[1];
        while (i--) {
            tok_t* t = file->locals[i+2];
            // unfold(t, 0);
            if (t->type==var_decl&&!strncmp(t->start, tok.start, min(t->len, tok.len)))
                tok.type = var_ident;
            else if (t->type==type_decl&&!strncmp(t->start, tok.start, min(t->len, tok.len)))
                tok.type = type_ident;
            else if (t->type==func_decl&&!strncmp(t->start, tok.start, min(t->len, tok.len)))
                tok.type = func_ident;
            if (tok.type!=ident) break;
        }
    } if (file->tstack) {
        size_t i = (size_t)file->tstack[1];
        while (i--) {
            tok_t* t = file->tstack[i+2];
            if (t->type==var_decl&&!strncmp(t->start, tok.start, min(t->len, tok.len)))
                tok.type = var_ident;
            else if (t->type==func_decl&&!strncmp(t->start, tok.start, min(t->len, tok.len)))
                tok.type = func_ident;
            if (tok.type!=ident) break;
        }
    }

    return rtok(proj, &tok);
}