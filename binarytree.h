#ifndef BINARYTREE_H
#define BINARYTREE_H
#include<iostream>
#include<vector>

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
    BinaryTree(const BinaryTree &other_tree);
    void insert(const T& value);
    void remove(const T& value);
    bool contains(const T& value);
    T getLargest();
    T getSmallest();
    void removeLargest();
    void removeSmallest();
    void printAll();
    void print(TreeNode<T>* current);
private:
    TreeNode<T>* root;
    T getLargest(TreeNode<T>* current);
    T getSmallest(TreeNode<T>* current);
    void removeLargest(TreeNode<T>* current);
    void removeSmallest(TreeNode<T>* current);
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

template<class T>
BinaryTree<T>::BinaryTree(const BinaryTree<T> &other_tree){
    TreeNode<T> other_node = other_tree.root;
    root = new TreeNode<T>(other_node.data);

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
        parent = current;
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
    if (parent->data > value){
        parent->left = new TreeNode<T>(value, parent);
    }
    else{
        parent->right = new TreeNode<T>(value, parent);
    }
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
        if(current->left !=nullptr){
            T largest_on_left = getLargest(current->left);
            removeLargest(current->left);
            current->data = largest_on_left;
        }
        else if(current->right != nullptr){
            T smallest_on_right = getSmallest(current->right);
            removeSmallest(current->right);
            current->data = smallest_on_right;
        }
        else{
            if(current == root){
                delete root;
                root = nullptr;
            }
            else if(current == current->parent->left){
                current->parent->left = nullptr;
                delete current;
            }
            else{
                current->parent->right = nullptr;
                delete current;
            }
        }
    }
}

template<class T>
T BinaryTree<T>::getLargest(){
    return getLargest(root);
}

template<class T>
T BinaryTree<T>::getLargest(TreeNode<T>* current){
    return current->right != nullptr ? getLargest(current->right) : current->data;
}

template<class T>
T BinaryTree<T>::getSmallest(){
    return getSmallest(root);
}
template<class T>
T BinaryTree<T>::getSmallest(TreeNode<T>* current){
    return current->left != nullptr ? getLargest(current->left) : current->data;
}

template<class T>
void BinaryTree<T>::removeLargest(){
    if(root == nullptr){
        return;
    }
    removeLargest(root);
}

template<class T>
void BinaryTree<T>::removeLargest(TreeNode<T>* current){
    if(root->right == nullptr){
        current = root->left;
        delete root;
        root = current;
        return;
    }
    while(current->right != nullptr){
        current = current->right;
    }
    current->parent->right = nullptr;
    delete current;
}

template<class T>
void BinaryTree<T>::removeSmallest(){
    if(root == nullptr){
        return;
    }
    removeSmallest(root);
}

template<class T>
void BinaryTree<T>::removeSmallest(TreeNode<T>* current){
    if(root->left == nullptr){
        current = root->right;
        delete root;
        root = current;
        return;
    }
    while(current->left != nullptr){
        current = current->left;
    }
    current->parent->left = nullptr;
    delete current;
}

// input: value to search for
// output: returns true if tree contains value
template<class T>
bool BinaryTree<T>::contains(const T &value){
    if(root == nullptr){
        return false;
    }
    TreeNode<T>* current = root;
    while (current != nullptr){
        if(current->data == value){
            return true;
        }
        else if(current->data > value){
            current = current->left;
        }
        else{
            current = current->right;
        }
    }
    return false;
}

template<class T>
void BinaryTree<T>::printAll(){
    print(root);
}

// prints current's left (smaller) descendents, then prints current, then prints current's right descendents
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
