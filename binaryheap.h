/************************************************************************************
**                                                                                 **
**  MIT License                                                                    **
**                                                                                 **
**  Copyright (c) 2017 Lucas Frey                                                  **
**                                                                                 **
**  Permission is hereby granted, free of charge, to any person obtaining          **
**  a copy of this software and associated documentation files (the "Software"),   **
**  to deal in the Software without restriction, including without limitation      **
**  the rights to use, copy, modify, merge, publish, distribute, sublicense,       **
**  and/or sell copies of the Software, and to permit persons to whom the          **
**  Software is furnished to do so, subject to the following conditions:           **
**                                                                                 **
**  The above copyright notice and this permission notice shall be included        **
**  in all copies or substantial portions of the Software.                         **
**                                                                                 **
**  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS        **
**  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    **
**  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
**  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
**  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
**  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
**  SOFTWARE.                                                                      **
**                                                                                 **
************************************************************************************/

#ifndef BinaryHeap_H_
#define BinaryHeap_H_
#include <functional>

////////////////////////////////////////////////////////////////////////////////////////
// DECLARATIONS
////////////////////////////////////////////////////////////////////////////////////////

template <class T, class CompareFunc = std::greater<T>>
class BinaryHeap {
 public:
    // default constructor
    BinaryHeap();
    // copy constructor
    BinaryHeap(const BinaryHeap& other);
    // destructor
    ~BinaryHeap();
    // adds value to the heap
    void push(T value);
    // removes and returns element at the top of the heap
    T pop();
    // returns the element at the top of the heap
    T top();
    // returns number of elements in heap
    int getSize();
    // returns true if the current heap array is full
    inline bool isFull();
    // resizes and frees up unused capacity allocated for array
    inline void shrinkToFit();

 private:
    // grows the heap array
    void grow();
    //pointer to heap array of elements
    T* data;
    int capacity;
    int size;
    // comparison functor used to define the order of the heap
    CompareFunc comp;
};

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS
///////////////////////////////////////////////////////////////////////////////

template<class T, class CompareFunc>
BinaryHeap<T, CompareFunc>::BinaryHeap() {
    capacity = 20;
    size = 0;
    data = new T[capacity];
}

template<class T, class CompareFunc>
BinaryHeap<T, CompareFunc>::BinaryHeap(const BinaryHeap &other) {
    capacity = other.capacity;
    size = other.size;
    data = new T[capacity];

    T* temp_data = new T[size];
    for (int i = 0; i < size; i++) {
        temp_data[i] = other.data[i];
    }
    for (int i = 0; i < size; i++) {
        push(temp_data[other.data]);
    }
}

template<class T, class CompareFunc>
BinaryHeap<T, CompareFunc>::~BinaryHeap() {
    delete data;
}

template<class T, class CompareFunc>
void BinaryHeap<T, CompareFunc>::push(T value) {
    if (isFull()) {
        grow();
    }

    int parent = size;
    data[size++] = value;

    while (parent != 0 && comp(data[(parent - 1)/2], data[parent])) {
        int t_parent = (parent-1)/2;
        T temp_value = data[parent];
        data[parent] = data[t_parent];
        data[t_parent] = temp_value;
        parent = t_parent;
    }
}

template<class T, class CompareFunc>
T BinaryHeap<T, CompareFunc>::pop() {
    if (size == 0) {
        std::cout << "ERROR: Trying to pop from an empty heap!" << std::endl;
        return T();
    }
    T to_return = data[0];
    size--;
    data[0] = data[size];
    int current = 0;
    while (current < size) {
        int next = current*2;
        if (comp(data[current], data[next+1])) {
                if (comp(data[next+1], data[next+2])) {
                    int temp = data[current];
                    data[current] = data[next+2];
                    data[next+2] = temp;
                    current = next+2;
                } else {
                    int temp = data[current];
                    data[current] = data[next+1];
                    data[next+1] = temp;
                    current = next+1;
                }
        } else if (comp(data[current], data[next+2])) {
            int temp = data[current];
            data[current] = data[next+2];
            data[next+2] = temp;
            current = next+2;
        } else {
            break;
        }
    }
    return to_return;
}

template<class T, class CompareFunc>
T BinaryHeap<T, CompareFunc>::top() {
    return data[0];
}

template<class T, class CompareFunc>
int BinaryHeap<T,CompareFunc>::getSize() {
    return size;
}

template<class T, class CompareFunc>
bool BinaryHeap<T,CompareFunc>::isFull() {
    return size == capacity;
}

template<class T, class CompareFunc>
void BinaryHeap<T,CompareFunc>::shrinkToFit() {
    T* temp_data = new T[size];
    for (int i = 0; i < size; i++) {
        temp_data[i] = data[i];
    }
    delete data;
    data = temp_data;
    capacity = size;
}

template<class T, class CompareFunc>
void BinaryHeap<T,CompareFunc>::grow() {
    capacity = capacity * 2;
    T* temp_data = new T[capacity];
    for (int i = 0; i < size; i++) {
        temp_data[i] = data[i];
    }
    delete data;
    data = temp_data;
}

#endif // BinaryHeap_H_
