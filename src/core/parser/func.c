#include "kit.h"

struct node* parse_func(struct parser* p) {
    struct node* ast = p->ast;
    if (!expect(p, IDENT)) return NULL;
    struct node* node = calloc(1, sizeof(struct node));
    node->type=NODE_FUNC;
    node->token=p->tok;

    return auto_free(node);
}
