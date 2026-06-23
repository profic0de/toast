#include "kit.h"

static inline struct node parse_params(struct parser* p) {
    struct node* ast = p->ast;
    struct node params = {.children=NULL, .token={0}, .type=NODE_PARAMS};
    struct token tok;
    while ((tok=next(p)).type!=RIGHT_PAREN) {
        if (tok.type<=EOF) return (error(p->p->lf->path.text,tok.start - p->start, tok.len, "error: unexpected EOF"),(struct node){});

        if (!is_type[tok.type]) return (error(p->p->lf->path.text,tok.start - p->start, tok.len, "error: expected a type"),(struct node){});
        if (!expect(p, IDENT)) return (struct node){};

    }

    return params;
}

struct node* parse_func(struct parser* p) {
    struct node* ast = p->ast;
    if (!expect(p, IDENT)) return NULL;
    struct node node = {.type=NODE_FUNC, .token=p->tok};
    if (!expect(p, LEFT_PAREN)) return NULL;
    struct node params = parse_params(p);
    if (!params.type) return NULL;

    // TODO: Finish function body parsing

    return &node;
}
