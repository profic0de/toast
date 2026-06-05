#include "kit.h"

int main() {
    string filepath = {.text="a/sd/",.len=strlen(filepath.text)};

    char path[filepath.len+2]; memset(path, 0, filepath.len+2);
    memcpy(path+1, filepath.text, filepath.len);

    char* filename = filepath.text+filepath.len;
    while (*--filename&&*filename!='/'); filename++;

    printf("%s\n", path[filepath.len]=='/'?"true":"false");
    printf("%d\n",(int)(filepath.text+filepath.len-filename));
    printf("\"%s\"\n", filename);

    int len = filepath.text+filepath.len-filename;
    byte shift = 0; if (path[filepath.len]!='/') shift++;
    char full_path[filepath.len+shift+4+len]; memset(full_path, 0, filepath.len+shift+4+len);
    memcpy(full_path+1, filepath.text, filepath.len);

    full_path[filepath.len+shift] = '/';
    memcpy(full_path+filepath.len+shift+1, filename, len);
    memcpy(full_path+filepath.len+shift+1+len, ".t", 2);

    printf("%s\n",full_path+1);

    return 0;
}