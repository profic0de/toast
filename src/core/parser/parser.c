#include "kit.h"

char* type_to_char(enum token_type token_type) {
    static char str[32];
    for (size_t i = 0; i < sizeof(str); i++) str[i] = 0;

    switch (token_type) {
    #define case_macro(val) case val: return (strcpy(str, #val), str);

    case_macro(EOF);
    case_macro(NUMBER);
    case_macro(FLOAT);
    case_macro(KEYWORD);
    case_macro(SYMBOL);
    case_macro(STRING);
    case_macro(PATH);
    case_macro(WORD);
    case_macro(OPERATOR);
    case_macro(BSTRING);
    case_macro(NSTRING);
    case_macro(LEFT_PAREN);
    case_macro(IDENT);
    case_macro(COLON);
    case_macro(KW_VAR); case_macro(KW_FUNC); case_macro(KW_LET); case_macro(KW_OBJ)
    case_macro(KW_SELF); case_macro(KW_RETURN); case_macro(KW_BREAK); case_macro(KW_IF)
    case_macro(KW_WAIT); case_macro(KW_YIELD); case_macro(KW_WHILE);
    case_macro(RIGHT_PAREN);case_macro(LEFT_BRACKET);case_macro(RIGHT_BRACKET);case_macro(LEFT_BRACE);
    case_macro(RIGHT_BRACE);case_macro(SEMICOLON);case_macro(COMMA);case_macro(DOT)
    
    case_macro(OPER_PLUS);case_macro(OPER_MINUS);case_macro(OPER_STAR);case_macro(OPER_SLASH);case_macro(OPER_PERCENT);
    case_macro(OPER_INCREMENT);case_macro(OPER_DECREMENT);
    case_macro(OPER_ASSIGN);case_macro(OPER_ADD_ASSIGN);case_macro(OPER_SUB_ASSIGN);case_macro(OPER_MUL_ASSIGN);case_macro(OPER_DIV_ASSIGN);case_macro(OPER_MOD_ASSIGN);
    case_macro(OPER_EQUAL);case_macro(OPER_NOT_EQUAL);
    case_macro(OPER_LESS);case_macro(OPER_LESS_EQUAL);case_macro(OPER_GREATER);case_macro(OPER_GREATER_EQUAL);
    case_macro(OPER_LOGICAL_AND);case_macro(OPER_LOGICAL_OR);
    case_macro(OPER_BITWISE_AND);case_macro(OPER_BITWISE_OR);case_macro(OPER_BITWISE_XOR);case_macro(OPER_BITWISE_NOT);
    case_macro(OPER_AND_ASSIGN);case_macro(OPER_OR_ASSIGN);case_macro(OPER_XOR_ASSIGN);
    case_macro(OPER_LEFT_SHIFT);case_macro(OPER_RIGHT_SHIFT);
    case_macro(OPER_LEFT_SHIFT_ASSIGN);case_macro(OPER_RIGHT_SHIFT_ASSIGN);

    #undef case_macro
    default:
        return (strcpy(str, "[ ]"), str);
    }
    return str;
}

int parse_file(size_t fd, struct project* project) {
    struct file* file = project->lf;
    // value(file->path.text);
    size_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char* buffer = malloc(size+1), *ptr = buffer, *end = ptr; {
        size_t len = read(fd, buffer, size);
        if (size != len) return (free(buffer),1);
        end += len;
    } buffer[size] = 0;

    struct token token;

    int r = 0;

    struct parser p = {.buffer=&ptr, .start=buffer, .end=end, .p=project};

    while (!r) {
        project->lf=file;
        if ((token = next(&p)).type==ERR||token.type==EOF) break;
        int parse_func(struct parser* p);
        
        if (token.type==KW_FUNC) r += parse_func(&p);
        // ptr = token.start;
        // if (token.start)
        // print("%.*s, type: %s (%s)",(int)token.len, token.start, type_to_char(token.type), file->name.text);
    }
    // print("end of %s", file->name.text);

    free(buffer);

    if (token.type==ERR) return 1;
    return 0;
}
