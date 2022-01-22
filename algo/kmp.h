#ifndef __KMP_H
#define __KMP_H

#include <string.h>

namespace KMP {
static int *buildNext(char *p) {
    int len = strlen(p);
    int *next = new int[len];
    next[0] = -1;
    int t = -1;
    int j = 0;
    while (j < len - 1) {
        if (t < 0 || p[j] == p[t]) {
            j++;
            t++;
            next[j] = p[j] == p[t] ? next[t] : t;
        } else {
            t = next[t];
        }
    }
    return next;
}

int match(char *str, char *p) {
    int *next = buildNext(p);
    int len = strlen(str);
    int pLen = strlen(p);
    int t = 0;
    int j = 0;
    while (j < len && t < pLen) {
        if (t < 0 || str[j] == p[t]) {
            j++;
            t++;
        } else {
            t = next[t];
        }
    }
    return j - t;
}
}

#endif