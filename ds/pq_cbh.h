#ifndef __PQ_CBH_H
#define __PQ_CBH_H

#include "pq.h"

template <typename T>
class PQ_CBH : public PQ<T>, public Vector<T> {
   private:
    static inline int parent(int r) {
        return (r - 1) / 2;
    }
    static inline int lc(int r) {
        return 2 * r + 1;
    }
    static inline int rc(int r) {
        return 2 * r + 2;
    }
    inline int inHeap(int r) {
        return r >= 0 && r < this->_size;
    }
    inline int properParent(int p) {
        int lc = this->lc(p);
        int rc = this->rc(p);
        int child;
        if (inHeap(lc)) {
            if (inHeap(rc)) {
                child = this->_data[lc] > this->_data[rc] ? lc : rc;
            } else {
                child = lc;
            }
        } else {
            if (inHeap(rc)) {
                child = rc;
            } else {
                return p;
            }
        }
        return this->_data[p] > this->_data[child] ? p : child;
    }
    void swap(int a, int b) {
        T tmp = this->_data[a];
        this->_data[a] = this->_data[b];
        this->_data[b] = tmp;
    }
    int precolateUp(int r) {
        while (r > 0) {
            int p = parent(r);
            if (this->_data[p] >= this->_data[r]) {
                break;
            }
            swap(p, r);
            r = p;
        }
        return r;
    }
    int percolateDown(int r) {
        int parent = properParent(r);
        while (parent != r) {
            swap(r, parent);
            r = parent;
            parent = properParent(r);
        }
        return r;
    }

   public:
    T getMax() {
        return this->_data[0];
    }
    void insert(T e) {
        Vector<T>::insert(e);
        precolateUp(this->_size - 1);
    }
    T delMax() {
        T tmp = getMax();
        this->_size--;
        this->_data[0] = this->_data[this->_size];
        percolateDown(0);
        return tmp;
    }
};

#endif