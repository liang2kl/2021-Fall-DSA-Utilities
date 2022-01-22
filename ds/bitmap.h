#ifndef __BITMAP_H
#define __BITMAP_H

#include <string.h>

class BitMap {
    char *_bits;
    unsigned long long _size;
public:
    BitMap(unsigned long long size): _size(size) {
        _bits = new char[_size / 8 + 1]();
    }
    int size() { return _size; }
    inline char bit(unsigned long long x) {
        return (*(_bits + x / 8) >> (7 - x % 8)) & 0x1;
    }
    inline void flipBit(unsigned long long x) {
        *(_bits + x / 8) ^= (1 << (7 - x % 8));
    }
    inline void setBit(unsigned long long x) {
        *(_bits + x / 8) |= (1 << (7 - x % 8));
    }
    inline void resetBit(unsigned long long x) {
        setBit(x);
        flipBit(x);
    }
    inline void reset() {
        memset(_bits, 0, _size / 8 + 1);
    }
    ~BitMap() {
        delete[] _bits;
    }
};

#endif