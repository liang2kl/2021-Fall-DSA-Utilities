#ifndef __BST_H
#define __BST_H

#include "bintree.h"
#include <tuple>

template <typename T>
class BST: public BinTree<T> {
private:
    using Position = BinNode<T> *;
protected:
    Position connect34(Position a, Position b, Position c,
                   Position t1, Position t2, Position t3, Position t4) {
        a->lc = t1;
        if (t1) t1->parent = a;
        a->rc = t2;
        if (t2) t2->parent = a;
        c->lc = t3;
        if (t3) t3->parent = c;
        c->rc = t4;
        if (t4) t4->parent = c;
        b->lc = a;
        b->rc = c;
        a->parent = b;
        c->parent = b;
        // b should be the last to update
        this->updateHeight(a);
        this->updateHeight(c);
        this->updateHeight(b);
        return b;
    }
    Position rotateAt(Position v) {
        Position p = v->parent;
        Position g = p->parent;
        if (BinTree<T>::isRC(v)) {
            if (BinTree<T>::isRC(p)) {
                p->parent = g->parent;
                return BST<T>::connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
            } else {
                v->parent = g->parent;
                return BST<T>::connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
            }
        } else {
            if (BinTree<T>::isRC(p)) {
                v->parent = g->parent;
                return BST<T>::connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
            } else {
                p->parent = g->parent;
                return BST<T>::connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
            }
        }
    }
public:
    virtual std::tuple<Position, Position> search(const T& e) {
        if (!this->_root || e == this->_root->data) {
            return std::make_tuple(this->_root, nullptr);
        }
        Position node = this->_root;
        while (node) {
            Position next = (e < node->data) ? node->lc : node->rc;
            if (!next || next->data == e) {
                return std::make_tuple(next, node);
            }
            node = next;
        }
        assert(false);
        return std::make_tuple(nullptr, nullptr);
    }

    virtual Position insert(const T&e) {
        auto result = search(e);
        Position node = std::get<0>(result);
        Position parent = std::get<1>(result);
        if (node) {
            return node;
        }
        if (!parent) {
            this->_root = new BinNode<T>(e, nullptr);
            this->_size = 1;
            this->updateHeight(this->_root);
            return this->_root;
        } else if (e < parent->data) {
            return BinTree<T>::insert(e, parent);
        } else {
            return BinTree<T>::insert(parent, e);
        }
    }

    virtual bool remove(const T& e) {
        auto result = search(e);
        Position node = std::get<0>(result);
        Position parent = std::get<1>(result);
        if (!node) {
            return false;
        }
        result = removeAt(node);
        this->_size--;
        this->updateHeightAbove(std::get<1>(result));
        return true;
    }

    std::tuple<Position, Position> removeAt(Position node) {
        Position removed = node;
        Position succ = nullptr;
        if (!removed->lc) {
            succ = removed->rc;
        } else if (!removed->rc) {
            succ = removed->lc;
        } else {
            Position replace = removed->succ();

            removed->data = replace->data;

            succ = replace->rc;
            removed = replace;
        }

        this->fromParentTo(removed) = succ;

        if (succ) {
            succ->parent = removed->parent;
        }
        return std::make_tuple(succ, removed->parent);
    }
};

#endif