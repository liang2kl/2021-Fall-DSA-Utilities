#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

template <typename T>
inline T max(T a, T b) {
    return a < b ? b : a;
}

template <typename T>
inline T min(T a, T b) {
    return a > b ? b : a;
}

#endif