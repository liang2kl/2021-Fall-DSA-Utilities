#ifndef __SWAP_H
#define __SWAP_H

namespace util {
template <typename T>
void swap(T &a, T &b) {
    T tmp = a;
    a = b;
    b = tmp;
}
}

#endif