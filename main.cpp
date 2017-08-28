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

#include <iostream>
#include <fstream>
#include <ctime>
#include "stringtrie.h"
#include "binarytree.h"
#include "linkedlist.h"

using namespace std;

void linkedListTest();
void binaryTreeTest();
void trieTest();
void storeBookInTrie(StringTrie &book);
void printLicense();

int main()
{
   printLicense();
   int choice = -1;
   while(choice != 0)
   {
       cout << "Select data structure:\n";
       cout << "0 - Exit\n";
       cout << "1 - Linked List\n";
       cout << "2 - Binary Tree\n";
       cout << "3 - String Trie\n";
       cout << "\n";
       cin >> choice;
       cout << endl;
       switch(choice){
           case 0:
               break;
           case 1:
               linkedListTest();
               break;
           case 2:
               binaryTreeTest();
               break;
           case 3:
               trieTest();
               break;
           default:
               cout << "Invalid choice!\n";
               break;
       }
   }
}

void linkedListTest(){
    LinkedList<int> my_list;
    int choice = -1;
    int list_input = 0;
    int index = 0;

    while(choice != 0)
    {
        cout << "----------------Linked List Test----------------" << endl;
        cout << "Select test method:\n";
        cout << "0 - Exit\n";
        cout << "1 - Insert number at beginning\n";
        cout << "2 - Insert number at end\n";
        cout << "3 - Insert number at index\n";
        cout << "4 - Remove number at beginning\n";
        cout << "5 - Remove number at end\n";
        cout << "6 - Remove number at index\n";
        cout << "7 - Find element\n";
        cout << "8 - Print elements\n";
        cin >> choice;
        cout << endl;
        switch(choice){
            case 0:
                break;
            case 1:
                cout << "Enter a number to add: ";
                cin >> list_input;
                my_list.insertStart(list_input);
                break;
            case 2:
                cout << "Enter a number to add: ";
                cin >> list_input;
                my_list.insertEnd(list_input);
                break;
            case 3:
                cout << "Enter a number to add: ";
                cin >> list_input;
                cout << "Enter an index: ";
                cin >> index;
                my_list.insertAt(list_input, index);
                break;
            case 4:
                cout << "Removing " << my_list.removeFirst() << " from the start\n";
                break;
            case 5:
                cout << "Removing " << my_list.removeLast() << " from the end\n";
                break;
            case 6:
                cout << "Enter an index: ";
                cin >> index;
                my_list.removeAt(index);
                break;
            case 7:
                cout << "Enter a number to find: ";
                cin >> list_input;
                if(my_list.contains(list_input)) {
                    cout << list_input << " is in the list\n";
                } else {
                    cout << list_input << " is not in the list\n";
                }
                break;
            case 8:
                my_list.print();
                break;
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
}

void trieTest(){
    int choice = -1;
    string temp_word = "";
    StringTrie book;
    while(choice != 0)
    {
        cout << "\n----------------Trie Test----------------" << endl;
        cout << "Select test method:\n";
        cout << "0 - Exit\n";
        cout << "1 - Import book\n";
        cout << "2 - Add word\n";
        cout << "3 - Print all\n";
        cout << "4 - Print all with prefix\n";
        cout << "5 - Search for word in trie\n";
        cout << "6 - Remove word\n";
        cout << "7 - Remove all with prefix\n";
        cin >> choice;
        printf("\r");
        switch(choice){
            case 0:
                break;
            case 1:
                storeBookInTrie(book);
            break;
            case 2:
                cout << "Enter word to add: ";
                cin >> temp_word;
                book.addWord(temp_word);
            break;
            case 3:
                book.printAll();
                break;
            case 4:
                cout << "Enter prefix: ";
                cin >> temp_word;
                book.printAllWithPrefix(temp_word);
                break;
            case 5:
                cout << "Enter word to search for: ";
                cin >> temp_word;
                if(book.contains(temp_word)){
                    std::cout << temp_word << " is in the trie\n";
                }
                else{
                    std::cout << temp_word << " is not in the trie\n";
                }
                break;
            case 6:
                cout << "Enter word to remove: ";
                cin >> temp_word;
                book.remove(temp_word);
                break;
            case 7:
                cout << "Enter prefix: ";
                cin >> temp_word;
                book.removeAllWithPrefix(temp_word);
                break;
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
}

void binaryTreeTest(){

    BinaryTree<int> myBinaryTree;
    int choice = -1;
    int tree_input = 0;

    while(choice != 0)
    {
        cout << "----------------Binary Tree Test----------------" << endl;
        cout << "Select test method:\n";
        cout << "0 - Exit\n";
        cout << "1 - Add number\n";
        cout << "2 - Remove number\n";
        cout << "3 - Find number\n";
        cout << "4 - Get smallest number\n";
        cout << "5 - Get largest number\n";
        cout << "6 - Print numbers in ascending order\n";
        cin >> choice;
        cout << endl;
        switch(choice){
            case 0:
                break;
            case 1:
                cout << "Enter number to add: ";
                cin >> tree_input;
                myBinaryTree.insert(tree_input);
                break;
            case 2:
                cout << "Enter number to remove: ";
                cin >> tree_input;
                myBinaryTree.remove(tree_input);
                break;
            case 3:
                cout << "Enter element to search for: ";
                cin >> tree_input;
                if(myBinaryTree.contains(tree_input)){
                    cout << tree_input << " is in the tree\n";
                }
                else{
                    cout << tree_input << " is not in the tree\n";
                }
                break;
            case 4:
                cout << "Smallest element in tree: " << myBinaryTree.getSmallest() << endl;
                break;
            case 5:
                cout << "Largest element in tree: " << myBinaryTree.getLargest() << endl;
                break;
            case 6:
                myBinaryTree.printAll();
                break;
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
}

void storeBookInTrie(StringTrie &book){

    ifstream myFile;
    myFile.open("GreatExpectations.txt");
    if(!myFile.is_open()){
        cout << "File in trieTest() didn't open!\n";
    }
    int wordCount = 1;
    int pageCount = 1;
    char c;
    string temp_word;
    clock_t start = clock();
    while(!myFile.eof()){
        myFile >> temp_word;
        if(temp_word == "----------------------------------------"){
            pageCount++;
            wordCount = 1;
        }
        else{
            c = temp_word[0];
            if(c < 90 && c > 63){
                c = c + 32;
            }
            temp_word = c + temp_word.substr(1, temp_word.size() - 1);
            book.addWord(temp_word);
            wordCount++;
        }
    }
    clock_t duration = clock() - start;
    myFile.close();
    cout << "Time taken to build trie: " << duration / (double)CLOCKS_PER_SEC << " seconds." << endl;
    cout << "Number of unique keys in trie: " << book.getNumberUniqueWords() << endl;
    cout << "Number of total keys in trie: " << book.getNumberTotalWords() << endl;
}

void printLicense() {
    cout <<
    " ******************************************************************************\n"
    "*                                                                              *\n"
    "*                                    MIT License                               *\n"
    "*                                                                              *\n"
    "*  Copyright (c) 2017 Lucas Frey                                               *\n"
    "*                                                                              *\n"
    "*  Permission is hereby granted, free of charge, to any person obtaining       *\n"
    "*  a copy of this software and associated documentation files (the             *\n"
    "*  \"Software\"), to deal in the Software without restriction, including         *\n"
    "*  without limitation the rights to use, copy, modify, merge, publish,         *\n"
    "*  distribute, sublicense, and/or sell copies of the Software, and to permit   *\n"
    "*  persons to whom the Software is furnished to do so, subject to the          *\n"
    "*  following conditions:                                                       *\n"
    "*                                                                              *\n"
    "*  The above copyright notice and this permission notice shall be included     *\n"
    "*  in all copies or substantial portions of the Software.                      *\n"
    "*                                                                              *\n"
    "*  THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,             *\n"
    "*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF          *\n"
    "*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.      *\n"
    "*  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY        *\n"
    "*  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,        *\n"
    "*  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE           *\n"
    "*  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                      *\n"
    "*                                                                              *\n"
    "*  Scroll up for license...                                                    *\n"
    "*                                                                              *\n"
    "********************************************************************************\n"
    "*                                                                              *\n"
    "*                              Data Structures                                 *\n"
    "*  Author: Lucas Frey                                                          *\n"
    "*                                                                              *\n"
    " ******************************************************************************\n\n";
}
