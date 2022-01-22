#ifndef __UFSET_H
#define __UFSET_H

#include "vector.h"

template <typename T>
struct UFSetNode {
    int parent;
    T data;
};

template <typename T>
class UFSet: public Vector<UFSetNode<T>> {
protected:
    inline int rootParent(int rank) {
        int parent = this->_data[rank].parent;
        if (parent == rank) {
            return rank;
        } else {
            parent = this->rootParent(parent);
            this->_data[rank].parent = parent;  // Path compression
            return parent;
        }
    }
public:
    UFSet(int size): Vector<UFSetNode<T>>(size) {
        for (int i = 0; i < size; i++) {
            this->_data[i].parent = i;
        }
    }
    virtual void union_(int a, int b) {
        a = rootParent(a);
        b = rootParent(b);
        if (a == b) {
            return;
        }
        this->_data[a].parent = b;
    }
    int find(int ele) {
        return this->_data[ele].parent;
    }
    T& data(int rank) {
        return this->_data[rank].data;
    }
};

#endif