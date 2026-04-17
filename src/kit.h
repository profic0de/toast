#ifndef KIT_H
#define KIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define print(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif