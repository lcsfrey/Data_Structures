#include <iostream>
#include <fstream>
#include <ctime>
#include "StringTrie.h"
#include "binarytree.h"

using namespace std;

void TrieTest();
void BinaryTreeTest();
void storeBookInTrie(StringTrie &book);

int main()
{
   cout << "----------------Data Structures----------------" << endl;
   cout << "-----------------By Lucas Frey-----------------" << endl;
   int choice = -1;
   while(choice != 0)
   {
       cout << "Select data structure:\n";
       cout << "0 - Exit\n";
       cout << "1 - String Trie\n";
       cout << "2 - Binary Tree\n";
       cin >> choice;
       cout << endl;
       switch(choice){
           case 0:
               break;
           case 1:
               TrieTest();
               break;
           case 2:
               BinaryTreeTest();
               break;
           default:
               cout << "Invalid choice!\n";
               break;
       }
   }
}

void TrieTest(){
    int choice = -1;
    string temp_word = "";
    StringTrie book;
    while(choice != 0)
    {
        cout << "----------------Trie Test----------------" << endl;
        cout << "Select test method:\n";
        cout << "0 - Exit\n";
        cout << "1 - Import book\n";
        cout << "2 - Add word\n";
        cout << "3 - Print all\n";
        cout << "4 - Print all with prefix\n";
        cout << "5 - Search for word in trie\n";
        cout << "6 - Get length of shortest word\n";
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
                cout << "Length of shortest word is: " << book.getLengthOfShortestWord() << endl;
                break;
            case 7:
                break;
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
}

void BinaryTreeTest(){

    BinaryTree<int> myBinaryTree;
    int choice = -1;
    int tree_input = 0;

    while(choice != 0)
    {
        cout << "----------------Binary Tree Test----------------" << endl;
        cout << "Select test method:\n";
        cout << "0 - Exit\n";
        cout << "1 - Add element\n";
        cout << "2 - Remove element\n";
        cout << "3 - Find element\n";
        cout << "4 - Get smallest element\n";
        cout << "5 - Get largest element\n";
        cin >> choice;
        cout << endl;
        switch(choice){
            case 0:
                break;
            case 1:
                cout << "Input element to add: ";
                cin >> tree_input;
                myBinaryTree.insert(tree_input);
                break;
            case 2:
                cout << "Input element to remove: ";
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
                cout << "Smallest element in tree: " << myBinaryTree.getSmallest();
                break;
            case 5:
                cout << "Largest element in tree: " << myBinaryTree.getLargest();
                break;
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
}

void storeBookInTrie(StringTrie &book){

    ifstream myFile;
    myFile.open("words.txt");
    if(!myFile.is_open()){
        cout << "File in TrieTest() didn't open!\n";
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
