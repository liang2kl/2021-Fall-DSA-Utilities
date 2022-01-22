#ifndef __BINTREE_H
#define __BINTREE_H

#include <cassert>
#include <functional>

#include "../util/common.h"
#include "queue.h"
#include "stack.h"

#ifdef RBTREE
namespace RB {
enum RBColor { RED, BLACK };
}
#endif
template <typename T>
struct BinNode {
    T data;
    BinNode *parent = nullptr;
    BinNode *lc = nullptr;
    BinNode *rc = nullptr;
    int height = 0;
#ifdef RBTREE
    RB::RBColor color;
#endif
    BinNode(BinNode *parent = nullptr) : parent(parent) {}
    BinNode(const T &ele, BinNode *parent) : data(ele), parent(parent) {}
    int size() {
        int s = 1;
        if (lc) s += lc->size();
        if (rc) s += rc->size();
        return s;
    }
    BinNode *insertAsLC(const T &ele) {
        lc = new BinNode(ele, this);
        lc->parent = this;
        return lc;
    }
    BinNode *insertAsRC(const T &ele) {
        rc = new BinNode(ele, this);
        rc->parent = this;
        return rc;
    }
    void insertAsLC(BinNode *node) {
        lc = node;
        node->parent = this;
    }
    void insertAsRC(BinNode *node) {
        rc = node;
        node->parent = this;
    }

    BinNode *succ() {
        BinNode *current = nullptr;
        if (rc) {
            current = rc;
            while (current->lc) {
                current = current->lc;
            }
            return current;
        } else if (parent) {
            current = this;
            while (current) {
                if (current->parent && current->parent->lc == this) {
                    return current->parent;
                }
                current = current->parent;
            }
        }
        return current;
    }

    BinNode *pred() {
        BinNode *node = this;
        if (lc) {
            node = lc;
            while (node->rc) {
                node = node->rc;
            }
            return node;
        }
        while (node->parent) {
            if (node->parent->rc == node) {
                return node->parent;
            }
            node = node->parent;
        }
        return nullptr;
    }

    void travPre(std::function<void(BinNode *)> action) {
        Stack<BinNode *> s;
        BinNode *current = this;

        while (true) {
            while (current) {
                action(current);
                if (current->rc) {
                    s.push(current->rc);
                }
                current = current->lc;
            }
            if (s.empty()) { break; }
            current = s.pop();
        }
    }
    void travIn(std::function<void(BinNode *)> action) {
        Stack<BinNode *> s;
        BinNode *current = this;

        while (true) {
            while (current) {
                s.push(current);
                current = current->lc;
            }
            if (s.empty()) { break; }

            current = s.pop();
            action(current);
            current = current->rc;
        }
    }

    void travPost(std::function<void(BinNode *)> action) {
        Stack<BinNode *> s;
        BinNode *current = this;
        s.push(this);

        while (!s.empty()) {
            if (s.top() != current->parent) {
                while ((current = s.top())) {
                    if (current->lc) {
                        if (current->rc) {
                            s.push(current->rc);
                        }
                        s.push(current->lc);
                    } else {
                        s.push(current->rc);
                    }
                }
                s.pop();
            }
            current = s.pop();
            action(current);
        }
    }

    void travLevel(std::function<void(BinNode *)> action) {
        Queue<BinNode *> q;
        q.enqueue(this);

        while (!q.empty()) {
            BinNode *current = q.dequeue();
            action(current);
            if (current->lc) {
                q.enqueue(current->lc);
            }
            if (current->rc) {
                q.enqueue(current->rc);
            }
        }
    }
};

template <typename T>
class BinTree {
protected:
    using Position = BinNode<T> *;
#ifndef BINTREE_PRIVATE_ACCESS
    Position _root = nullptr;
    int _size = 0;
#endif
    /**
     * Update height of a single node
     * 
     * @param node: The node to be updated 
     */
    virtual int updateHeight(Position node) {
        return node->height = 1 + 
            max(stature(node->lc), stature(node->rc));
    }
    /**
     * Update height of all ancestors of a node
     * 
     * @param node: The lowest node to be updated 
     */
    void updateHeightAbove(Position node) {
        while (node) {
            int height = node->height;
            if (height == updateHeight(node)) {
                break;
            }
            node = node->parent;
        }
    }
    /**
     * Return the height of a subtree
     * 
     * @param node: The input node, can be null
     * @return -1 if input is null, otherwise its height
     */
    inline virtual int stature(Position node) {
        return node ? node->height : -1;
    }
    /**
     * Whether the node is the right child of its parent.
     * The node and the parent of the node must not be null.
     * 
     * @param node: The input node, cannot be null
     */
    inline static bool isLC(Position node) {
        return node->parent->lc == node;
    }
    /**
     * @see isLC
     */
    inline static bool isRC(Position node) {
        return !isLC(node);
    }
    /**
     * Return the reference of the pointer that:
     * (1) the input node's parent used to point to the node,
     * if it has a parent
     * (2) point to the root node, if it does not have a parent
     * 
     * @param node: The input node, can be any non-null node
     */
    inline Position &fromParentTo(Position node) {
        if (node == _root) {
            return _root;
        }
        if (isLC(node)) {
            return node->parent->lc;
        } else {
            return node->parent->rc;
        }
    }

public:
#ifdef BINTREE_PRIVATE_ACCESS
    Position _root = nullptr;
    int _size = 0;
#endif
    int size() const { return _size; }
    Position root() const { return _root; }
    void setRoot(const T &ele) { 
        _root = new BinNode<T>(ele, nullptr);
        _size = 1;
    }
    Position insert(Position node, const T &ele) {
        assert(!node->rc);
        _size++;
        node->insertAsRC(ele);
        updateHeightAbove(node);
        return node->rc;
    }
    Position insert(const T &ele, Position node) {
        assert(!node->lc);
        _size++;
        node->insertAsLC(ele);
        updateHeightAbove(node);
        return node->lc;
    }
    int remove(Position node) {
        fromParentTo(node) = nullptr;
        updateHeight(node->parent);
        int n = removeAt(node);
        _size -= n;
        return n;
    }
    int removeAt(Position node) {
        if (!node) { 
            return 0;
        }
        int n = 1 + removeAt(node->lc) + removeAt(node->rc);
        // delete node;
        return n;
    }
    // TODO: Attach, detach
};

#endif