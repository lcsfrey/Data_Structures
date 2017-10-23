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
#include "binaryheap.h"
#include "skiplist.h"
#include "wordtrie.h"

using namespace std;
void testMenu();
void linkedListTest();
void binaryTreeTest();
void trieTest();
void heapTest();
void skipListTest();
void storeBookInTrie(StringTrie &book);
void printLicense();

int main() {
    srand(time(0));
    printLicense();
    testMenu();
    return 0;
}

void testMenu() {
    int choice = -1;
    while (choice != 0)
    {
        cout << "Select data structure:\n";
        cout << "0 - Exit\n";
        cout << "1 - Linked List\n";
        cout << "2 - Skip List\n";
        cout << "3 - Binary Tree\n";
        cout << "4 - String Trie\n";
        cout << "5 - Binary Heap";
        cout << "\n";
        cin >> choice;
        cout << endl;
        switch (choice){
            case 0:
                break;
            case 1:
                linkedListTest();
                break;
            case 2:
                skipListTest();
                break;
            case 3:
                binaryTreeTest();
                break;
            case 4:
                trieTest();
                break;
            case 5:
                heapTest();
            break;
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
}

void linkedListTest() {
    LinkedList<int> my_linked_list;
    int choice = -1;
    int list_input = 0;
    int index = 0;

    while (choice != 0) {
        cout << "----------------Linked List Test----------------" << endl;
        cout << "Select test method:\n";
        cout << "0 - Exit\n";
        cout << "1 - Insert number at beginning\n";
        cout << "2 - Insert number at end\n";
        cout << "3 - Insert number at index\n";
        cout << "4 - Remove number at beginning\n";
        cout << "5 - Remove number at end\n";
        cout << "6 - Remove number at index\n";
        cout << "7 - Find number\n";
        cout << "8 - Print numbers\n";
        cin >> choice;
        cout << endl;
        switch (choice){
            case 0:
                break;
            case 1:
                cout << "Enter a number to add: ";
                cin >> list_input;
                my_linked_list.insertStart(list_input);
                break;
            case 2:
                cout << "Enter a number to add: ";
                cin >> list_input;
                my_linked_list.insertEnd(list_input);
                break;
            case 3:
                cout << "Enter a number to add: ";
                cin >> list_input;
                cout << "Enter an index: ";
                cin >> index;
                my_linked_list.insertAt(list_input, index);
                break;
            case 4:
                cout << "Removing " << my_linked_list.removeFirst() << " from the start\n";
                break;
            case 5:
                cout << "Removing " << my_linked_list.removeLast() << " from the end\n";
                break;
            case 6:
                cout << "Enter an index: ";
                cin >> index;
                my_linked_list.removeAt(index);
                break;
            case 7:
                cout << "Enter a number to find: ";
                cin >> list_input;
                if(my_linked_list.contains(list_input)) {
                    cout << list_input << " is in the list\n";
                } else {
                    cout << list_input << " is not in the list\n";
                }
                break;
            case 8:
                my_linked_list.print();
                break;
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
}

void trieTest() {
    int choice = -1;
    string temp_word = "";
    StringTrie my_string_trie;
    while (choice != 0)
    {
        cout << "\n----------------Trie Test----------------" << endl;
        cout << "Select test method:\n";
        cout << "0 - Exit\n";
        cout << "1 - Import book\n";
        cout << "2 - Add word\n";
        cout << "3 - Print all\n";
        cout << "4 - Print all with prefix\n";
        cout << "5 - Print all ordered by occurance\n";
        cout << "6 - Search for word in trie\n";
        cout << "7 - Remove word\n";
        cout << "8 - Remove all with prefix\n";
        cout << "9 - Get longest word\n";
        cout << "10 - Get length of shortest word\n";
        cout << "11 - Get length of longest word\n\n";
        cin >> choice;
        switch (choice){
            case 0:
                break;
            case 1:
                storeBookInTrie(my_string_trie);

            break;
            case 2:
                cout << "Enter word to add: ";
                cin >> temp_word;
                my_string_trie.addWord(temp_word);
            break;
            case 3:
                my_string_trie.printAll();
                break;
            case 4:
                cout << "Enter prefix: ";
                cin >> temp_word;
                my_string_trie.printAllWithPrefix(temp_word);
                break;
            case 5: {

                std::vector<pair<string, int>> words_by_frequency = my_string_trie.m_record->getOrderedWords(&my_string_trie);
                int size = words_by_frequency.size();
                string word = "";
                cout << "\nWords By Occurences\n----------------------------\n";
                for (int i = 0; i < size / 2; i++) {
                    pair<string, int> pair_word_freq = words_by_frequency[i];
                    cout << "word " << i << ": " << pair_word_freq.first
                         << "\nfrequency: " << pair_word_freq.second << endl << endl;
                }
                break;
            }
            case 6:
                cout << "Enter word to search for: ";
                cin >> temp_word;
                if(my_string_trie.contains(temp_word)){
                    std::cout << temp_word << " is in the trie\n";
                }
                else{
                    std::cout << temp_word << " is not in the trie\n";
                }
                break;
            case 7:
                cout << "Enter word to remove: ";
                cin >> temp_word;
                my_string_trie.remove(temp_word);
                break;
            case 8:
                cout << "Enter prefix: ";
                cin >> temp_word;
                my_string_trie.removeAllWithPrefix(temp_word);
                break;
            case 9:
                cout << "Longest word: " << my_string_trie.getLongestWord() << endl;
                break;
            case 10:
                cout << "Length of shortest word: " << my_string_trie.getLengthOfShortestWord() << endl;
                break;
            case 11:
                cout << "Length of longest word: " << my_string_trie.getLengthOfLongestWord() << endl;
                break;

            default:
                cout << "Invalid choice!\n";
                break;
        }

      }

    }


void binaryTreeTest() {
    BinaryTree<int> my_binary_tree;
    int choice = -1;
    int tree_input = 0;

    while (choice != 0)
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
        switch (choice){
            case 0:
                break;
            case 1:
                cout << "Enter number to add: ";
                cin >> tree_input;
                my_binary_tree.insert(tree_input);
                break;
            case 2:
                cout << "Enter number to remove: ";
                cin >> tree_input;
                my_binary_tree.remove(tree_input);
                break;
            case 3:
                cout << "Enter number to search for: ";
                cin >> tree_input;
                if(my_binary_tree.contains(tree_input)){
                    cout << tree_input << " is in the tree\n";
                }
                else{
                    cout << tree_input << " is not in the tree\n";
                }
                break;
            case 4:
                cout << "Smallest number in tree: " << my_binary_tree.getSmallest() << endl;
                break;
            case 5:
                cout << "Largest number in tree: " << my_binary_tree.getLargest() << endl;
                break;
            case 6:
                my_binary_tree.printAll();
                break;
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
}

void heapTest() {
    BinaryHeap<int, std::less<int>> my_max_heap;
    BinaryHeap<int> my_min_heap;
    srand(time(0));
    int choice = -1;
    int heap_input = 0;

    while (choice != 0) {
        cout << "----------------Min & Max Heap Test----------------" << endl;
        cout << "Select test method:\n";
        cout << "0 - Exit\n";
        cout << "1 - Add number\n";
        cout << "2 - Remove top\n";
        cout << "3 - Get size of heap\n";
        cout << "4 - Get smallest number\n";
        cout << "5 - Get largest number\n";
        cout << "6 - Add random values\n";
        cin >> choice;
        cout << endl;
        switch (choice){
            case 0:
                break;
            case 1:
                cout << "Enter number to add: ";
                cin >> heap_input;
                my_max_heap.push(heap_input);
                my_min_heap.push(heap_input);
                break;
            case 2:
                cout << "Removing from max heap: " << my_max_heap.pop() << endl;
                cout << "Removing from min heap: " << my_min_heap.pop() << endl;
                break;
            case 3:
                cout << "Size of heap: " << my_min_heap.getSize() << endl;
                break;
            case 4:
                cout << "Smallest number is: " << my_min_heap.top() << endl;
                break;
            case 5:
                cout << "Largest number is: " << my_max_heap.top() << endl;
                break;
            case 6: {
            int count = 0;
            int max_value = 0;
            int min_value = 100;
            cout << "Enter number of values to add: ";
            cin >> count;
            cout << "Enter max value: ";
            cin >> max_value;
            cout << "Enter min value: ";
            cin >> min_value;
            if (min_value > 0) {
                max_value += min_value;
            } else {
                max_value -= min_value;
            }
            for (int i = 0; i < count; i++) {
                int value = (rand() % max_value) + min_value;
                my_max_heap.push(value);
                my_min_heap.push(value);
            }
            break;
            }
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
}

void skipListTest() {
    SkipList<int> my_skiplist;
    int choice = -1;
    int list_input = 0;
    while (choice != 0)
    {
        cout << "----------------Skip List Test----------------" << endl;
        cout << "Select test method:\n";
        cout << "0 - Exit\n";
        cout << "1 - Insert number\n";
        cout << "2 - Remove number\n";
        cout << "3 - Find number\n";
        cout << "4 - Add random numbers\n";
        cout << "5 - Print numbers\n";
        cin >> choice;
        cout << endl;
        switch (choice){
            case 0:
                break;
            case 1:
                cout << "Enter a number to add: ";
                cin >> list_input;
                my_skiplist.insert(list_input);
                break;
            case 2:
                cout << "Enter a number to remove: ";
                cin >> list_input;
                my_skiplist.remove(list_input);
                break;
            case 3:
                cout << "Enter a number to find: ";
                cin >> list_input;
                if(my_skiplist.contains(list_input)) {
                    cout << list_input << " is in the list\n";
                } else {
                    cout << list_input << " is not in the list\n";
                }
                break;
            case 4: {
                int count = 0;
                int max_value = 0;
                int min_value = 100;
                cout << "Enter number of values to add: ";
                cin >> count;
                cout << "Enter max value: ";
                cin >> max_value;
                cout << "Enter min value: ";
                cin >> min_value;
                if (min_value > 0) {
                    max_value += min_value;
                } else {
                    max_value -= min_value;
                }
                for (int i = 0; i < count; i++) {
                    int value = (rand() % max_value) + min_value;
                    my_skiplist.insert(value);
                }
                break;
            }
            case 5:
                my_skiplist.print();
                break;
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
}

void storeBookInTrie(StringTrie &book) {

    string filename("GreatExpectations.txt");
    ifstream myFile;
    myFile.open(filename);
    if(!myFile.is_open()){
        cout << "File in trieTest() didn't open!\n";
    }
    cout << "Now Loading " << filename << "...\n";

    int wordCount = 1;
    int pageCount = 1;
    char c;
    string temp_word;
    clock_t start = clock();
    while (!myFile.eof()){
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
