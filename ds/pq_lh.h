#ifndef __PQ_LH_H
#define __PQ_LH_H

#include "pq.h"
#include "bintree.h"
#include "../util/swap.h"

#define npl height

template <typename T>
class PQ_LH: public PQ<T>, public BinTree<T> {
private:
    using Position = BinNode<T>*;
public:
    virtual void merge(PQ_LH<T> *other) {
        this->_size += other->size();
        this->_root = merge(this->_root, other->_root);
        other->_root = this->_root;
    }
    virtual Position merge(Position a, Position b) {
        if (!a) { return b; }
        if (!b) { return a; }
        if (a->data < b->data) {
            util::swap(a, b);
        }
        a->rc = merge(a->rc, b);
        a->rc->parent = a;
        if (!a->lc || a->lc->npl < a->rc->npl) {
            util::swap(a->lc, a->rc);
        }
        a->npl = a->rc ? 1 + a->rc->npl : 1;
        return a;
    }
    void insert(T e) {
        this->_root = merge(this->_root, new BinNode<T>(e, nullptr));
        this->_size++;
    }
    T getMax() {
        return this->_root->data;
    }
    T delMax() {
        T data = this->_root->data;
        if (this->_root->lc) {
            this->_root->lc->parent = nullptr;
        }
        if (this->_root->rc) {
            this->_root->rc->parent = nullptr;
        }
        this->_root = merge(this->_root->lc, this->_root->rc);
        this->_size--;
        return data;
    }
};

#undef npl

#endif