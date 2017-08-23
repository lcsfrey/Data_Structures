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

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include<iostream>

////////////////////////////////////////////////////////////////////////////////////////
// DECLARATIONS
////////////////////////////////////////////////////////////////////////////////////////

template<class T>
struct ListNode {
    T data;
    ListNode<T>* next;
    ListNode<T>* prev;
    ListNode();
    explicit ListNode(const T& value);
};

template<class T>
class LinkedList {
 public:
    LinkedList();
    explicit LinkedList(const LinkedList& other_list);
    ~LinkedList();

    LinkedList& operator=(const LinkedList& other_list);

    void insertStart(const T& value);
    void insertEnd(const T& value);
    void insertAt(const T& value, const int index);

    T removeFirst();
    T removeLast();
    T removeAt(const int index);

    bool contains(const T& value);
    inline T getFirst();
    inline T getLast();
    T getItemAt(const int index);
    inline int getLength() const;
    void print() const;

 private:
        ListNode<T>* head;
        ListNode<T>* tail;
        int length;
};

////////////////////////////////////////////////////////////////////////////////////////
// DEFINITIONS
////////////////////////////////////////////////////////////////////////////////////////
// initializes empty ListNode
template<class T>
ListNode<T>::ListNode() {
    next = nullptr;
    prev = nullptr;
}

// initializes ListNode
template<class T>
ListNode<T>::ListNode(const T& value) {
    data = value;
    next = nullptr;
    prev = nullptr;
}

// initializes empty LinkedList
template<class T>
LinkedList<T>::LinkedList() {
    head = new ListNode<T>();
    tail = new ListNode<T>();
    length = 0;
    head->next = tail;
    tail->prev = head;
}

// copy constructor
// initializes new LinkedList as a deep copy of existing LinkedList
template<class T>
LinkedList<T>::LinkedList(const LinkedList& other_list) {
    ListNode<T>* other_node = other_list.head->next;
    ListNode<T>* current = head;
    head = new ListNode<T>();
    tail = new ListNode<T>();
    current = current->next;
    while (other_node != other_list.tail) {
        current->next = new ListNode<T>(other_node->data);
        current->next->prev = current;
        current = current->next;
        other_node = other_node->next;
    }
    current->next = tail;
    tail->prev = current;
    other_list.length;
}

// releases memory allocated by LinkedList
template<class T>
LinkedList<T>::~LinkedList() {
    ListNode<T>* current = head->next;
    while (current != tail) {
        delete current->prev;
        current = current->next;
    }
    delete current->prev;
    delete current;
}

template<class T>
int LinkedList<T>::getLength() const {
    return length;
}

// prints list in order in the form,
// "Head -> data -> data -> data...."
template<class T>
void LinkedList<T>::print() const {
    ListNode<T>* current = head->next;
    std::cout << "Head -> ";
    while (current != tail->prev) {
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << current->data << std::endl;
}

// insert item at start of list
template<class T>
void LinkedList<T>::insertStart(const T &value) {
    ListNode<T>* temp = new ListNode<T>(value);
    temp->next = head->next;
    temp->next->prev = temp;
    temp->prev = head;
    head->next = temp;
    length++;
}

// insert item at end of list
template<class T>
void LinkedList<T>::insertEnd(const T &value) {
    ListNode<T>* temp = new ListNode<T>(value);
    temp->prev = tail->prev;
    temp->next = tail;
    tail->prev->next = temp;
    tail->prev = temp;
    length++;
}

// insert at certain length into list
// adds to beginning if length is 0
// adds to end if index is greater than length of list
template<class T>
void LinkedList<T>::insertAt(const T &value, const int index) {
    if (index >= length) {
        insertEnd(value);
    } else if (index == 0) {
        insertStart(value);
    } else {
        int temp_position = 0;
        ListNode<T>* current = head;
        ListNode<T>* temp = new ListNode<T>(value);
        while (temp_position < index) {
            current = current->next;
            temp_position++;
        }
        temp->prev = current;
        temp->next = current->next;
        current->next->prev = temp;
        current->next = temp;
        length++;
    }
}

// removes first item from array
// returns copy of item that was deleted
template<class T>
T LinkedList<T>::removeFirst() {
    T temp_data = head->next->data;
    if (length > 0) {
        ListNode<T>* to_delete = head->next;
        head->next = head->next->next;
        head->next->prev = head;
        delete to_delete;
        length--;
    }
    return temp_data;
}

// removes last item from array
// returns copy of item that was deleted
template<class T>
T LinkedList<T>::removeLast() {
    T temp_data = tail->prev->data;
    if (length > 0) {
        ListNode<T>* to_delete = tail->prev;
        tail->prev = to_delete->prev;
        to_delete->prev->next = tail;
        delete to_delete;
        length--;
    }
    return temp_data;
}

// remove element at index
// removes the last element if the index is greater than the length of the list
template<class T>
T LinkedList<T>::removeAt(const int index) {
    if (index >= length) {
        return removeLast();
    } else if (index == 0) {
        return removeFirst();
    } else {
        int temp_position = 0;
        ListNode<T>* to_delete = head->next;
        while (temp_position < index) {
            to_delete = to_delete->next;
            temp_position++;
        }
        to_delete->prev->next = to_delete->next;
        to_delete->next->prev = to_delete->prev;
        T data(to_delete->data);
        delete to_delete;
        length--;
        return data;
    }
}

template<class T>
bool LinkedList<T>::contains(const T &value) {
    ListNode<T>* current = head->next;
    while (current != tail) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template<class T>
T LinkedList<T>::getFirst() {
    return head->next->data;
}

template<class T>
T LinkedList<T>::getLast() {
    return tail->prev->data;
}

template<class T>
T LinkedList<T>::getItemAt(const int index) {
    if (index >= length) {
        return tail->prev->data;
    } else if (index == 0) {
        return head->next->data;
    } else {
        int temp_position = 0;
        ListNode<T>* current = head->next;
        while (temp_position < index) {
            current = current->next;
            temp_position++;
        }
        return current->data;
    }
}

#endif  // LINKEDLIST_H_
