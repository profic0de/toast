#ifndef AST_H
#define AST_H
#include "kit.h"

enum node_type {
    NODE_ROOT,
    NODE_PARAMS,
    NODE_DECL,
    NODE_PARAM,
    NODE_FUNC,
    NODE_VAR,
    NODE_CONST
};

struct node {
    enum node_type type;
    struct token token;
    struct node** children;  // null terminated
};

inline static struct node* make_node(enum node_type type, struct token token) {
    struct node* n = malloc(sizeof(struct node));
    n->type = type;
    n->token = token;
    n->children = NULL;
    return n;
}

inline static void node_add_child(struct node* parent, struct node* child) {
    parent->children = array_append(parent->children, child);
}
#endif