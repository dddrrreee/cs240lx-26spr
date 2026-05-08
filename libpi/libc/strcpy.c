#include "rpi.h"

char *strcpy(char * s1, const char * s2) {
    char *s = s1;
    unsigned n = 0;
    while (n < 4096 && (*s++ = *s2++) != 0)
        n++;
    return s1;
}
