#ifndef __QUEUE_H
#define __QUEUE_H

#include "vector.h"

template <typename T>
class Queue: public Vector<T> {
public:
    void enqueue(const T &ele) { this->insert(ele); }
    T dequeue() { return this->removeAt(0); }
    inline T front() { return this->_data[0];  }
};

#endif