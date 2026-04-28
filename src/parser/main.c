#include "kit.h"

char** tokens;

char* handle_token(char** bytes) {
    char* token = bytes ? *bytes : NULL;
    static char* previous_token = NULL;

    if (previous_token) free(previous_token);

    if (token||token[0]=='>') {
        
        return strdup("unimplemented");
    }

    previous_token = token;
    return NULL;
}