#include "kit.h"

uint8_t is_type[TOKEN_TYPES];

static inline struct node parse_decl(struct parser* p) {
    struct node decl = {.children=NULL, .token={0}, .type=NODE_DECL};
    struct token tok = next(p);

    if (tok.type<=EOF) return (error(p->p->lf->path.text,tok.start - p->start, tok.len, "error: unexpected EOF"),(struct node){});

    if (!is_type[tok.type]) return (error(p->p->lf->path.text,tok.start - p->start, tok.len, "error: expected a type"),(struct node){});
    if (!expect(p, IDENT)) return (struct node){};

    return decl;
}