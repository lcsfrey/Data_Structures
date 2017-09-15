#ifndef SKIPLIST_H
#define SKIPLIST_H
#include <vector>

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

template<class T>
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

 private:
     SkipListNode<T>* head;
     SkipListNode<T>* tail;
     int length;
};


///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS
///////////////////////////////////////////////////////////////////////////////

template<class T>
SkipList<T>::SkipList() {
    length = 0;
    tail = new SkipListNode<T>();
    head = new SkipListNode<T>();
}

template<class T>
SkipList<T>::~SkipList() {
    SkipListNode<T>* current = head.at(0);
    while (current != tail) {
        SkipListNode<T>* next = current->next.at(0);
        delete current;
        current = next;
    }
    delete current;
}

template<class T>
void SkipList<T>::insert(const T &value) {
    int current_level = head->next.size() - 1;
    SkipListNode<T>* current = head->next.back();
    SkipListNode<T>* next = current->next.at(current_level);
}

template<class T>
int SkipList<T>::getLength() const {
    return length;
}

template<class T>
bool SkipList<T>::isEmpty() const {
    return length == 0;
}





#endif // SKIPLIST_H
