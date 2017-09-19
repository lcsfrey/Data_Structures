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
#ifndef SKIPLIST_H_
#define SKIPLIST_H_
#include <vector>
#include <random>
#include <functional>

///////////////////////////////////////////////////////////////////////////////
// DECLARATIONS
///////////////////////////////////////////////////////////////////////////////

template<class T>
struct SkipListNode {
    T data;
    std::vector<SkipListNode<T>*> next;
    std::vector<SkipListNode<T>*> prev;
    SkipListNode();
    explicit SkipListNode(const T& value);
};

template<class T, class CompareFunc = std::less<T>>
class SkipList {
 public:
     SkipList();
     explicit SkipList(const SkipList& other_list);
     ~SkipList();

     SkipList& operator=(const SkipList& other_list);

     void insert(const T& value);
     void remove(const T& value);
     bool contains(const T& value) const;
     T front();
     T back();
     inline int getLength() const;
     inline bool isEmpty() const;
     void print() const;

 private:
     SkipListNode<T> *findNode(const T& value) const;
     SkipListNode<T>* head;
     SkipListNode<T>* tail;
     std::default_random_engine m_generator;
     std::bernoulli_distribution m_distribution;
     int length;
     CompareFunc comp;
};

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS
///////////////////////////////////////////////////////////////////////////////

template<class T>
SkipListNode<T>::SkipListNode() {
    next.push_back(nullptr);
    prev.push_back(nullptr);
}

template<class T>
SkipListNode<T>::SkipListNode(const T &value) {
    data = value;
    next.push_back(nullptr);
    prev.push_back(nullptr);
}

template<class T, class CompareFunc>
SkipList<T, CompareFunc>::SkipList() : m_distribution(.5) {
    length = 0;
    tail = new SkipListNode<T>();
    head = new SkipListNode<T>();
    head->next[0] = tail;
    tail->prev[0] = head;
}

template<class T, class CompareFunc>
SkipList<T, CompareFunc>::~SkipList() {
    SkipListNode<T>* current = head;
    SkipListNode<T>* next;
    while (current != tail) {
        next = current->next.front();
        delete current;
        current = next;
    }
    delete current;
}

template<class T, class CompareFunc>
void SkipList<T, CompareFunc>::insert(const T &value) {
    int current_level = head->next.size() - 1;
    std::vector<SkipListNode<T>*> jump_list(head->next.size());
    SkipListNode<T>* current = head;
    SkipListNode<T>* next = current->next.back();
    while (current_level > 0) {
        if (next != tail && comp(current->next[current_level]->data, value)) {
            current = next;
            next = next->next.at(current_level);
        } else {
            jump_list[current_level] = current;
            current_level--;
            next = current->next.at(current_level);
        }
    }
    while (next != tail && comp(current->next[current_level]->data, value)) {
        current = next;
        next = next->next[0];
    }
    SkipListNode<T>* node_to_add = new SkipListNode<T>(value);
    node_to_add->next[0] = next;
    next->prev[0] = node_to_add;
    node_to_add->prev[0] = current;
    current->next[0] = node_to_add;

    int size = jump_list.size();
    int index = 1;
    while (m_distribution(m_generator)) {
        if (index < size) {
            current = jump_list[index];
            next = current->next[index];

            node_to_add->next.push_back(next);
            next->prev[index] = node_to_add;

            node_to_add->prev.push_back(current);
            current->next[index] = node_to_add;
            index++;
        } else {
            head->next.push_back(node_to_add);
            node_to_add->prev.push_back(head);
            node_to_add->next.push_back(tail);
            tail->prev.push_back(node_to_add);
        }
    }
    length++;
}

template<class T, class CompareFunc>
void SkipList<T, CompareFunc>::remove(const T &value) {
    SkipListNode<T>* node_to_delete = findNode(value);
    if (node_to_delete != nullptr) {
        for (int i = node_to_delete->next.size() - 1; i >= 0; i--) {
            node_to_delete->prev[i]->next[i] = node_to_delete->next[i];
            node_to_delete->next[i]->prev[i] = node_to_delete->prev[i];
        }
        delete node_to_delete;
        length--;
    }
}

template<class T, class CompareFunc>
bool SkipList<T, CompareFunc>::contains(const T &value) const {
    SkipListNode<T>* current = findNode(value);
    if (current != nullptr) {
        return true;
    } else {
        return false;
    }
}

template<class T, class CompareFunc>
int SkipList<T, CompareFunc>::getLength() const {
    return length;
}

template<class T, class CompareFunc>
bool SkipList<T, CompareFunc>::isEmpty() const {
    return length == 0;
}

template<class T, class CompareFunc>
void SkipList<T, CompareFunc>::print() const {
    SkipListNode<T>* current = head->next[0];
    std::cout << "Head -> ";
    while (current != tail->prev[0]) {
        std::cout << current->data << " -> ";
        current = current->next[0];
    }
    std::cout << current->data << " -> Tail" << std::endl;
}

template<class T, class CompareFunc>
SkipListNode<T>* SkipList<T, CompareFunc>::findNode(const T &value) const {
    int current_level = head->next.size() - 1;
    SkipListNode<T>* current = head;
    do {
        if (current->data == value) {
            return current;
        } else if (current->next[current_level] != tail &&
                   (comp(current->next[current_level]->data, value) ||
                    current->next[current_level]->data == value)) {
            current = current->next[current_level];
        } else {
            current_level--;
        }
    } while (current != tail && current_level >= 0);
    return nullptr;
}

#endif  // SKIPLIST_H_


