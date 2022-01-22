#ifndef __PQ_H
#define __PQ_H

#include "vector.h"

template <typename T>
class PQ {
    virtual void insert(T) = 0;
    virtual T getMax() = 0;
    virtual T delMax() = 0;
};

#endif