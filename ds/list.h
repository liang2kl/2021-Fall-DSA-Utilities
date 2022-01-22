#ifndef __LIST_H
#define __LIST_H

#include <functional>

template <typename T>
struct ListNode {
    T data;
    ListNode *pred = nullptr;
    ListNode *succ = nullptr;
    ListNode() {}
    ListNode(const T &e, ListNode<T> *pred = nullptr, ListNode<T> *succ = nullptr) : data(e), pred(pred), succ(succ) {}
    ListNode *insertAsPred(ListNode *node) {
        node->pred = pred;
        node->succ = this;
        pred->succ = node;
        pred = node;
        return node;
    }
    ListNode *insertAsSucc(ListNode *node) {
        node->pred = this;
        node->succ = succ;
        succ->pred = node;
        succ = node;
        return node;
    }

    ListNode *insertAsPred(const T &e) {
        auto node = new ListNode<T>(e, pred, this);
        return insertAsPred(node);
    }
    ListNode *insertAsSucc(const T &e) {
        auto node = new ListNode<T>(e, this, succ);
        return insertAsSucc(node);
    }
};

template <typename T>
class List {
protected:
    using Node = ListNode<T>;
    using Position = Node *;

    Position _header;
    Position _trailer;
    int _size;

    Position _positionAt(int rank) {
        Node *current = _header->succ;
        for (int i = 0; i < rank; i++) {
            if (current == _trailer) {
                return nullptr;
            }
            current = current->succ;
        }
        return current;
    }

   public:
    List() {
        _size = 0;
        _header = new Node();
        _trailer = new Node();
        _header->succ = _trailer;
        _trailer->pred = _header;
    }
    int size() { return _size; }
    Position first() { return _header->succ; }
    Position last() { return _trailer->pred; }
    Position find(const T &e) {
        Position p = first();
        while (p != _trailer) {
            if (p->data == e) return p;
            p = p->succ;
        }
        return nullptr;
    }
    void insertAsFirst(Position node) {
        _size++;
        _header->insertAsSucc(node);
    }
    void insertAsLast(Position node) {
        _size++;
        _trailer->insertAsPred(node);
    }
    Position insertAsFirst(const T &e) {
        _size++;
        return _header->insertAsSucc(e);
    }
    Position insertAsLast(const T &e) {
        _size++;
        return _trailer->insertAsPred(e);
    }
    void insertAsPred(Position p, Position node) {
        _size++;
        p->insertAsPred(node);
    }
    void insertAsSucc(Position p, Position node) {
        _size++;
        p->insertAsSucc(node);
    }
    Position insert(const T &e, Position p) {
        _size++;
        return p->insertAsPred(e);
    }
    Position insert(Position p, const T &e) {
        _size++;
        return p->insertAsSucc(e);
    }
    void insert(Position node, int rank) {
        Position pos = this->_positionAt(rank);
        insertAsPred(pos, node);
    }
    T remove(Position p) {
        _size--;
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        auto e = p->data;
        // delete p;
        return e;
    }
    void traverse(std::function<void(T &, bool &)> action, bool reversed = false) {
        bool flag = false;
        if (!reversed) {
            Position p = first();
            while (p != _trailer && !flag) {
                action(p->data, flag);
                p = p->succ;
            }
        } else {
            Position p = last();
            while (p != _header && !flag) {
                action(p->data, flag);
                p = p->pred;
            }
        }
    }
    Position positionAt(int rank) {
        Position pos = _positionAt(rank);
        return pos == _trailer ? nullptr : pos;
    }
};

#endif