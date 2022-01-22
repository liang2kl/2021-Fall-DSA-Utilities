#ifndef __RBTREE_H
#define __RBTREE_H
#define RBTREE

#include "../util/common.h"
#include "bst.h"

template <typename T>
class RBTree: public BST<T> {
private:
    using Position = BinNode<T> *;

    inline bool isBlack(Position x) {
        return !x || x->color == RB::BLACK;
    }

    inline bool isRed(Position x) {
        return !isBlack(x);
    }

    inline bool blackHeightUpdated(Position x) {
        return this->stature(x->lc) == this->stature(x->rc) &&
               x->height == (isBlack(x) ? this->stature(x->lc) + 1 : this->stature(x->lc));
    }

    inline int stature(Position node) override {
        return node ? node->height : 0;
    }

    int updateHeight(Position x) override {
        int stature = max(this->stature(x->lc), this->stature(x->rc));
        x->height = isBlack(x) + stature;
        return x->height;
    }

    void solveDoubleRed(Position x) {
        if (this->_root == x) {
            x->color = RB::BLACK;
            x->height++;
            return;
        }
        Position p = x->parent;
        if (isBlack(p)) {
            return;
        }
        Position g = p->parent;
        Position u = BinTree<T>::isRC(p) ? g->lc : g->rc;
        if (isBlack(u)) {
            if (this->isLC(p) == this->isLC(x)) {
                p->color = RB::BLACK;
            } else {
                x->color = RB::BLACK;
            }
            g->color = RB::RED;
            Position &ref = this->fromParentTo(g);
            ref = this->rotateAt(x);
        } else {
            p->color = RB::BLACK;
            p->height++;
            u->color = RB::BLACK;
            u->height++;
            if (this->_root != g) {
                g->color = RB::RED;
            }
            solveDoubleRed(g);
        }
    }

    void solveDoubleBlack(Position r, Position p) {
        if (!p) {
            return;
        }
        Position s = r == p->lc ? p->rc : p->lc;
        if (isBlack(s)) {
            Position t = nullptr;
            if (isRed(s->rc)) {
                t = s->rc;
            }
            if (isRed(s->lc)) {
                t = s->lc;
            }
            if (t) {
                RB::RBColor pColor = p->color;
                Position &ref = this->fromParentTo(p);
                ref = this->rotateAt(t);
                Position b = ref;
                if (b->lc) {
                    b->lc->color = RB::BLACK;
                    updateHeight(b->lc);
                }
                if (b->rc) {
                    b->rc->color = RB::BLACK;
                    updateHeight(b->rc);
                }
                b->color = pColor;
                updateHeight(b);
            } else {
                s->color = RB::RED;
                s->height--;
                if (isRed(p)) {
                    p->color = RB::BLACK;
                } else {
                    p->height--;
                    solveDoubleBlack(p, p->parent);
                }
            }
        } else {
            s->color = RB::BLACK;
            p->color = RB::RED;
            Position t = this->isLC(s) ? s->lc : s->rc;
            Position &ref = this->fromParentTo(p);
            ref = this->rotateAt(t);
            solveDoubleBlack(r, p);
        }
    }

public:
    Position insert(const T &ele) override {
        Position node = BST<T>::insert(ele);
        node->color = RB::RED;
        solveDoubleRed(node);
        return node;
    }

    bool remove(const T &ele) override {
        auto result = this->search(ele);
        Position node = std::get<0>(result);
        if (!node) {
            return false;
        }
        result = this->removeAt(node);
        this->_size--;

        if (this->_size == 0) {
            return true;
        }

        Position r = std::get<0>(result);
        Position p = std::get<1>(result);

        if (!p) {
            this->_root->color = RB::BLACK;
            updateHeight(this->_root);
            return true;
        }
        if (blackHeightUpdated(p)) {
            return true;
        }
        if (isRed(r)) {
            r->color = RB::BLACK;
            r->height++;
            return true;
        }
        solveDoubleBlack(r, p);
        return true;
    }
};

#endif
