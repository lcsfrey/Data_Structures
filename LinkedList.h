#ifndef LinkedList_H
#define LinkedList_H

#include<iostream>

////////////////////////////////////////////////////////////////////////////////////////
//DECLARATIONS
////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct ListNode{
    T data;
    ListNode<T>* next;
    ListNode<T>* prev;
    ListNode();
    ListNode(const T& value);
};

template<class T>
class LinkedList{
private:
    ListNode<T>* head;
    ListNode<T>* tail;
    int length;

public:
    LinkedList();
    LinkedList(const LinkedList& other_list);
    ~LinkedList();

    LinkedList& operator=(const LinkedList& other_list);

    int getLength() const;
    void print() const;

    void insertStart(const T& value);
    void insertEnd(const T& value);
    void insertAt(const T& value, const int index);

    T removeFirst();
    T removeLast();
    T removeAt(const int index);

    T getFirst();
    T getLast();
    T getItemAt();
};

////////////////////////////////////////////////////////////////////////////////////////
//DEFINITIONS
////////////////////////////////////////////////////////////////////////////////////////

template<class T>
ListNode<T>::ListNode(){
    next = nullptr;
    prev = nullptr;
}

template<class T>
ListNode<T>::ListNode(const T& value)
{
    data = value;
    next = nullptr;
    prev = nullptr;
}

template<class T>
LinkedList<T>::LinkedList()
{
    head = new ListNode<T>();
    tail = new ListNode<T>();
    length = 0;
    head->next = tail;
    tail->prev = head;
    head->prev = nullptr;
    tail->next = nullptr;
}

template<class T>
LinkedList<T>::~LinkedList(){
    ListNode<T>* current = head->next;
    while(current != tail)
    {
        delete current->prev;
        current = current->next;
    }
    delete current->prev;
    delete current;
}

template<class T>
int LinkedList<T>::getLength() const{
    return length;
}

template<class T>
void LinkedList<T>::print() const{
    ListNode<T>* current = head->next;
    std::cout << "Head -> ";
    while(current != tail->prev){
        std::cout << current->data << " -> ";
        current = current->next;
    }
    std::cout << current->data << std::endl;
}

template<class T>
void LinkedList<T>::insertStart(const T &value){
    ListNode<T>* temp = new ListNode<T>(value);
    temp->next = head->next;
    temp->next->prev = temp;
    temp->prev = head;
    head->next = temp;
    length++;
}

template<class T>
void LinkedList<T>::insertEnd(const T &value){
    ListNode<T>* temp = new ListNode<T>(value);
    temp->prev = tail->prev;
    temp->next = tail;
    tail->prev->next = temp;
    tail->prev = temp;
    length++;
}

template<class T>
void LinkedList<T>::insertAt(const T &value, const int index){
    if(index >= length){
        insertEnd(value);
    }
    else if(index == 0){
        insertStart(value);
    }
    else{
        int temp_position = 0;
        ListNode<T>* current = head;
        ListNode<T>* temp = new ListNode<T>(value);
        while(temp_position < index){
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

template<class T>
T LinkedList<T>::removeFirst(){
    T temp_data = head->next->data;
    if(length > 0){
        ListNode<T>* to_delete = head->next;
        head->next = head->next->next;
        head->next->prev = head;
        delete to_delete;
    }
    return temp_data;
}

template<class T>
T LinkedList<T>::removeLast(){
    T temp_data = tail->prev->data;
    if(length > 0){
        ListNode<T>* to_delete = tail->prev;
        tail->prev = head->prev->prev;
        tail->prev->next = tail;
        delete to_delete;
    }
    return temp_data;
}


#endif // LinkedList_H
