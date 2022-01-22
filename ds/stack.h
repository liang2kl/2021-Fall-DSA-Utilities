#ifndef __STACK_H
#define __STACK_H

#include "vector.h"

template<typename T>
class Stack: public Vector<T> {
public:
    void push(const T&ele) {
        this->insert(ele);
    }
    T pop() {
        this->_size--;
        return (*this)[this->_size];
    }
    T& top() {
        return this->last();
    }
};

#endif