#ifndef __VECTOR_H
#define __VECTOR_H

#include <functional>
#include <cstring>

#ifndef VECTOR_INIT_CAPA
#define VECTOR_INIT_CAPA 20
#endif

template <typename T>
class Vector {
protected:
    void _expand() {
        if (_size <= _capacity) return;
        T* newData = new T[_capacity * 2];
        memmove(newData, _data, _capacity * sizeof(T));
        _capacity *= 2;
        // delete[] _data;
        _data = newData;
    }
    int _capacity;
    T* _data;
    int _size;

public:
    Vector(int capacity = VECTOR_INIT_CAPA) {
        _capacity = capacity;
        _size = 0;
        _data = new T[capacity];
    }
    void insert(const T& ele) {
        insert(ele, _size);
    }
    void insert(const T& ele, int rank) {
        _size++;
        _expand();
        if (_size - 1 - rank > 0) {
            memmove(_data + rank + 1, _data + rank, (_size - 1 - rank) * sizeof(T));
        }
        _data[rank] = ele;
    }
    T removeAt(int rank) {
        T ele = _data[rank];
        if (_size - 1 - rank > 0) {
            memmove(_data + rank, _data + rank + 1, (_size - rank - 1) * sizeof(T));
        }
        _size--;
        return ele;
    }
    int size() { return _size; }
    bool empty() { return _size == 0; }
    int find(const T& ele) {
        for (int i = 0; i < _size; i++) {
            if (_data[i] == ele)
                return i;
        }
    }
    int binSearch(int lo, int hi, const T& ele) {
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (ele < _data[mid])
                hi = mid;
            else
                lo = mid + 1;
        }
        return lo - 1;
    }
    T& first() { return _data[0]; }
    T& last() { return _data[_size - 1]; }
    virtual T& operator[](int rank) { return _data[rank]; }
    void traverse(std::function<void(T&)> action, bool reverse = false) {
        traverse([&](T &ele, bool) { action(ele); }, reverse);
    }
    void traverse(std::function<void(T &, bool &)> action, bool reverse = false) {
        bool flag = false;
        if (!reverse) {
            for (int i = 0; i < _size && !flag; i++) {
                action(_data[i], flag);
            }
        } else {
            for (int i = _size - 1; i >= 0 && !flag; i--) {
                action(_data[i], flag);
            }
        }
    }

    // TODO: sort, deduplicate, uniquify
};

#endif