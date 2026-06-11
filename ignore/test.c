#include "kit.h"
#include "tools/ctype.h"
char str_box[UINT16_MAX+1];

int main() {
    char* buffer = "<=a>";
    char* end = buffer+strlen(buffer);

    uint8_t c = *buffer++;
    uint8_t i = 0;
    uint8_t buf[5] = {0};

    if (is_operator(c)) {
        memcpy(buf, buffer-1, min(sizeof(buf)-1, end-(buffer-1)));
        i=0; while (++i) if (!is_operator(buf[i])) {
            memset(buf+i, 0, end-(buffer+i-1));
            break;
        }
    }

    value((char*)buf);
    value(end-(buffer+i-1));

    return 0;
}