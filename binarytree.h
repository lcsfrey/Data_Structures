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

#ifndef BINARYTREE_H_
#define BINARYTREE_H_
#include<iostream>
#include<vector>

////////////////////////////////////////////////////////////////////////////////////////
// DECLARATIONS
////////////////////////////////////////////////////////////////////////////////////////

template<class T>
struct TreeNode {
    T data;
    TreeNode<T>* left;
    TreeNode<T>* right;
    TreeNode<T>* parent;
    // creates empty tree node
    TreeNode();
    // creates tree node containing value
    TreeNode<T>(const T& value, TreeNode<T>* temp_parent = nullptr);
};

template<class T>
class BinaryTree {
 public:
    // default constructor
    BinaryTree();
    // copy constructor
    explicit BinaryTree(const BinaryTree &other_tree);
    void insert(const T& value);
    // returns true if value is in the tree
    bool contains(const T& value);
    // returns largest element in tree
    inline T getLargest();
    // returns smallest element in tree
    inline T getSmallest();
    // removes value from tree
    void remove(const T& value);
    // removes largest element from tree
    void removeLargest();
    // removes smallest element from tree
    void removeSmallest();
    // prints all elements in tree
    void printAll();

 private:
    TreeNode<T>* root;
    // helper function that does inorder print of current node
    // and all decendents nodes
    inline void print(TreeNode<T>* current);
    // helper function that returns largest element in tree
    // rooted at current
    T getLargest(TreeNode<T>* current);
    // helper function that returns smallest element in tree
    // rooted at current
    T getSmallest(TreeNode<T>* current);
    // helper function that removes largest element in tree
    // rooted at current
    void removeLargest(TreeNode<T>* current);
    // helper function that removes smallest element in tree
    // rooted at current
    void removeSmallest(TreeNode<T> *current);
};

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS
///////////////////////////////////////////////////////////////////////////////

template<class T>
TreeNode<T>::TreeNode() {
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}


template<class T>
TreeNode<T>::TreeNode(const T& value, TreeNode<T>* temp_parent) {
    data = value;
    left = nullptr;
    right = nullptr;
    parent = temp_parent;
}

template<class T>
BinaryTree<T>::BinaryTree() {
    root = nullptr;
}

template<class T>
BinaryTree<T>::BinaryTree(const BinaryTree<T> &other_tree) {
    TreeNode<T> other_node = other_tree.root;
    root = new TreeNode<T>(other_node.data);
}

// insert node with value into the tree if it does not already exist
template<class T>
void BinaryTree<T>::insert(const T& value) {
    if (root == nullptr) {
        root = new TreeNode<T>(value, nullptr);
    }
    TreeNode<T>* parent = root;
    TreeNode<T>* current = root;
    while (current != nullptr) {
        parent = current;
        if (current->data == value) {
            return;
        } else if (current->data > value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (parent->data > value) {
        parent->left = new TreeNode<T>(value, parent);
    } else {
        parent->right = new TreeNode<T>(value, parent);
    }
}

// Removes value from tree if it exists
// Replaces value of that node with the largest element on the left of the right
template<class T>
void BinaryTree<T>::remove(const T &value) {
    if (root == nullptr) {
        return;
    }
    TreeNode<T>* current = root;
    while (current != nullptr) {
        if (current->data == value) {
            break;
        } else if (current->data > value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (current != nullptr) {
        if (current->left !=nullptr) {
            T largest_on_left = getLargest(current->left);
            removeLargest(current->left);
            current->data = largest_on_left;
        } else if (current->right != nullptr) {
            T smallest_on_right = getSmallest(current->right);
            removeSmallest(current->right);
            current->data = smallest_on_right;
        } else {
            if (current == root) {
                delete root;
                root = nullptr;
            } else if (current == current->parent->left) {
                current->parent->left = nullptr;
                delete current;
            } else {
                current->parent->right = nullptr;
                delete current;
            }
        }
    }
}

template<class T>
T BinaryTree<T>::getLargest() {
    return getLargest(root);
}

template<class T>
T BinaryTree<T>::getLargest(TreeNode<T>* current) {
    if (current->right != nullptr) return  getLargest(current->right);
    else
        return current->data;
}

template<class T>
T BinaryTree<T>::getSmallest() {
    return getSmallest(root);
}
template<class T>
T BinaryTree<T>::getSmallest(TreeNode<T>* current) {
    return current->left != nullptr ? getLargest(current->left) : current->data;
}

template<class T>
void BinaryTree<T>::removeLargest() {
    if (root == nullptr) {
        return;
    }
    removeLargest(root);
}

template<class T>
void BinaryTree<T>::removeLargest(TreeNode<T>* current) {
    if (root->right == nullptr) {
        current = root->left;
        delete root;
        root = current;
        return;
    }
    while (current->right != nullptr) {
        current = current->right;
    }
    current->parent->right = nullptr;
    delete current;
}

template<class T>
void BinaryTree<T>::removeSmallest() {
    if (root == nullptr) {
        return;
    }
    removeSmallest(root);
}

template<class T>
void BinaryTree<T>::removeSmallest(TreeNode<T> *current) {
    if (root->left == nullptr) {
        current = root->right;
        delete root;
        root = current;
        return;
    }
    while (current->left != nullptr) {
        current = current->left;
    }
    current->parent->left = nullptr;
    delete current;
}

// input: value to search for
// output: returns true if tree contains value

template<class T>
bool BinaryTree<T>::contains(const T &value) {
    if (root == nullptr) {
        return false;
    }
    TreeNode<T>* current = root;
    while (current != nullptr) {
        if (current->data == value) return true;
        else if (current->data > value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return false;
}

template<class T>
void BinaryTree<T>::printAll() {
    print(root);
    std::cout << std::endl;
}

// prints current's left descendents
// then prints current
// then prints current's right descendents

template<class T>
void BinaryTree<T>::print(TreeNode<T> *current) {
    if (current == nullptr) {
        return;
    }
    print(current->left);
    std::cout << current->data << " ";
    print(current->right);
}

#endif  // BINARYTREE_H_
