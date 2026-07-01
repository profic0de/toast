#include "kit.h"

// Examples of values:
// func(), func(stuff)
// ident, 1, "123"
struct node parse_term(struct parser* p) {
    struct node val = {.children=NULL, .token={0}, .type=NODE_CONST};
    struct token tok = next(p);

    switch (tok.type) {
    case ERR: case EOF: return (error(p->p->lf->path.text,tok.start - p->start, tok.len, "error: unexpected EOF"),(struct node){});
    
    case IDENT: {
        switch (next(p).type) {
        case LEFT_PAREN: //ident(
            val.type=NODE_CALL;
            struct node parse_params(struct parser* p);

            parse_params(p);

            break;
        
        case LEFT_BRACKET: //ident[
            val.type=NODE_INDEX;

            break;
        
        default: back(p);
        }
    }
    
    default:
        break;
    }

    return val;
}