#ifndef BINARYTREE_H
#define BINARYTREE_H
#include<iostream>

////////////////////////////////////////////////////////////////////////////////////////
//DECLARATIONS
////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct TreeNode{
    TreeNode();
    TreeNode(const T& value);
    T data;
    TreeNode* left;
    TreeNode* right;
};

////////////////////////////////////////////////////////////////////////////////////////
// DEFINITIONS
////////////////////////////////////////////////////////////////////////////////////////
template<class T>
class BinaryTree{
public:
    BinaryTree();
    void insert(const T& value);
    void printAll();
    void print(TreeNode<T>* current);
private:
    TreeNode<T>* root;
};

template<class T>
TreeNode<T>::TreeNode(){
    left = nullptr;
    right = nullptr;
}


template<class T>
TreeNode<T>::TreeNode(const T& value){
    data = value;
    left = nullptr;
    right = nullptr;
}

template<class T>
BinaryTree<T>::BinaryTree(){
    root = nullptr;
}

template<class T>
void BinaryTree<T>::insert(const T& value){
    if(root == nullptr){
        root = new TreeNode<T>(value);
    }
    TreeNode<T>* current = root;
    while (current != nullptr){
        if(current->data == value){
            return;
        }
        else if(current->data > value){
            current = current->left;
        }
        else{
            current = current->right;
        }
    }
    current = new TreeNode<T>(value);
}

template<class T>
void BinaryTree<T>::printAll(){
    printAll(root);
}

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
