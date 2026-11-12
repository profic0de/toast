#include "statement.h"

void check_if_declarable(tok_t* tok) {
    file_t* file = tok->file;
    char* msg = "This identifier is allready being used";
    if (tok->type==ident||tok->type==type_ident) return;
    if (tok->type==var_ident||tok->type==func_ident) {
        tok_t** tstack = file->tstack;
        if (!tstack||!(size_t)tstack[1]) lexer_error(tok, msg);
        size_t i = (size_t)tstack[1];
    // checking if anything with this name is declared in the current stack frame
        tok_t* t; while (i--) {
            t = tstack[i+2];
            if (t->type==var_decl&&!strncmp(t->start, tok->start, min(t->len, tok->len))) break;
            if (t->type==func_decl&&!strncmp(t->start, tok->start, min(t->len, tok->len))) break;
            if (t->type==sfs) break;
        } if (t->type!=sfs) lexer_error(tok, msg);
    }
}

void cpy_all(tok_t* dest, tok_t* src) {
    tok_t** contents = dest->contents;
    bool solid = dest->solid;
    memcpy(dest, src, sizeof(tok_t));
    dest->contents = contents;
    dest->solid = solid;
}

tok_t* lex_decl(proj_t* proj) {
    // <type> <ident>
    tok_t* decl = proj->last_tok, *tok=lex_ident(proj), *oper;
    check_if_declarable(tok);
    index_append((void***)&decl->contents, tok);
    cpy_all(decl, tok);
    decl->type = var_decl;
    if (!(oper=lex_operator(proj))) return decl;
    if (oper->type!=oper_semicolon) undo(oper);
    

    return decl;
}