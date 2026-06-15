#include <stdio.h>
#include <stdint.h>
#include "tools/ctype.h"

int main() {
    uint8_t* str = (uint8_t*)"1234.01123"-1;
    double f = 0;
    double s = 1;

    while (is_digit(*++str)) f = f*10+*str-'0';
    if (*str=='.') while (is_digit(*++str)) f += (s/=10)*(*str-'0');

    printf("%f\n",f);

    return 0;
}