#ifndef __AVL_H
#define __AVL_H

#include "bst.h"

template <typename T>
class AVL: public BST<T> {
private:
    using Position = BinNode<T> *;
    inline int balanceFactor(Position node) {
        return this->stature(node->lc) - this->stature(node->rc);
    }
    inline bool avlBalanced(Position node) {
        return balanceFactor(node) > -2 && balanceFactor(node) < 2;
    }
    inline Position tallerChild(Position parent) {
        return this->stature(parent->lc) >= this->stature(parent->rc) ? parent->lc : parent->rc;
    }
public:
    Position insert(const T &e) override {
        Position node = BST<T>::insert(e);
        for (Position a = node->parent; a; a = a->parent) {
            if (!avlBalanced(a)) {
                Position &ref = this->fromParentTo(a);
                ref = this->rotateAt(tallerChild(tallerChild(a)));
                break;
            } else {
                this->updateHeight(a);
            }
        }
        return node;
    }

    bool remove(const T &e) override {
        auto result = this->search(e);
        Position node = std::get<0>(result);
        Position parent = std::get<1>(result);
        if (!node) {
            return false;
        }

        auto removeResult = this->removeAt(node);
        this->_size--;

        for (Position a = std::get<1>(removeResult); a; a = a->parent) {
            if (!avlBalanced(a)) {
                Position &ref = this->fromParentTo(a);
                ref = this->rotateAt(tallerChild(tallerChild(a)));
                a = ref;
            }
            this->updateHeight(a);
        }
        return true;
    }
};

#endif