#ifndef BINARYTREE_H
#define BINARYTREE_H
#include<iostream>

////////////////////////////////////////////////////////////////////////////////////////
//DECLARATIONS
////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct TreeNode{
    T data;
    TreeNode<T>* left;
    TreeNode<T>* right;
    TreeNode<T>* parent;
    TreeNode();
    TreeNode<T>(const T& value, TreeNode<T>* temp_parent);
};

////////////////////////////////////////////////////////////////////////////////////////
// DEFINITIONS
////////////////////////////////////////////////////////////////////////////////////////
template<class T>
class BinaryTree{
public:
    BinaryTree();
    void insert(const T& value);
    void remove(const T& value);
    void printAll();
    void print(TreeNode<T>* current);
private:
    TreeNode<T>* root;
};

template<class T>
TreeNode<T>::TreeNode(){
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}


template<class T>
TreeNode<T>::TreeNode(const T& value, TreeNode<T>* temp_parent){
    data = value;
    left = nullptr;
    right = nullptr;
    parent = temp_parent;
}

// initializes empty Binary Tree
template<class T>
BinaryTree<T>::BinaryTree(){
    root = nullptr;
}

// insert node with value into the tree if it does not already exist
template<class T>
void BinaryTree<T>::insert(const T& value){
    if(root == nullptr){
        root = new TreeNode<T>(value, nullptr);
    }
    TreeNode<T>* parent = root;
    TreeNode<T>* current = root;
    while (current != nullptr){
        if(current->data == value){
            return;
        }
        else if(current->data > value){
            parent = current;
            current = current->left;
        }
        else{
            parent = current;
            current = current->right;
        }
    }
    current = new TreeNode<T>(value, parent);
}

// Removes value from tree if it exists
// Replaces value of that node with the largest element on the left of the right
template<class T>
void BinaryTree<T>::remove(const T &value){
    if(root == nullptr){
        return;
    }
    TreeNode<T>* current = root;
    while (current != nullptr){
        if(current->data == value){
            break;
        }
        else if(current->data > value){
            current = current->left;
        }
        else{
            current = current->right;
        }
    }
    if(current != nullptr)
    {
        TreeNode<T>* largest_on_left = current->left;
        while(largest_on_left->right != nullptr)
        {
            largest_on_left = largest_on_left->right;
        }
        largest_on_left->parent->right = largest_on_left->left;
        current->data = largest_on_left->data;
        delete largest_on_left;
    }
}

template<class T>
void BinaryTree<T>::printAll(){
    print(root);
}

// helper function that prints all nodes in order that are children of current
template<class T>
void BinaryTree<T>::print(TreeNode<T> *current){
    if(current == nullptr){
        return;
    }
    print(current->left);
    std::cout << current->data << ", ";
    print(current->right);
}




#endif // BINARYTREE_H
