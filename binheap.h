#ifndef BINHEAP_H
#define BINHEAP_H
#include <functional>

template <typename T, typename CompareFunc = std::greater<T>>
class BinHeap {
 public:
    BinHeap();
    void push(T value);
    T pop();
    T getMin();
    void removeMin();
    int getSize();

 private:
    inline bool isFull();
    void grow();
    T* data;
    int capacity;
    int size;
};

template<class T, typename CompareFunc>
BinHeap<T, CompareFunc>::BinHeap() {
    capacity = 20;
    size = 0;
    data = new T[capacity];
}

template<class T, typename CompareFunc>
void BinHeap<T,CompareFunc>::push(T value) {
    if (isFull()) {
        grow();
    }

    int parent = size;
    data[size++] = value;

    while (parent != 0 && CompareFunc(data[(parent - 1)/2], data[parent])) {
        int t_parent = (parent-1)/2;
        T temp_value = data[parent];
        data[parent] = data[t_parent];
        data[t_parent] = temp_value;
        parent = t_parent;
    }
}

template<class T, typename CompareFunc>
bool BinHeap<T,CompareFunc>::isFull() {
    return size == capacity;
}

template<class T, typename CompareFunc>
void BinHeap<T,CompareFunc>::grow() {
    capacity = capacity * 2;
    T* temp_data = new T[capacity];
    for (int i = 0; i < size; i++) {
        temp_data[i] = data[i];
    }
    delete data;
    data = temp_data;
}

#endif // BINHEAP_H
