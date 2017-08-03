#include <iostream>
#include <fstream>
#include <ctime>
#include "StringTrie.h"
#include "binarytree.h"

using namespace std;

void TrieTest();
void BinaryTreeTest();

int main()
{
   BinaryTreeTest();
}

void TrieTest(){
    cout << "\n----------------Trie Stats----------------" << endl;

    ifstream myFile;
    myFile.open("GreatExpectations.txt");
    if(!myFile.is_open()){
        cout << "File in TrieTest() didn't open!\n";
    }
    int wordCount = 1;
    int pageCount = 1;
    int choice = -1;
    char c;
    string tempWord;
    StringTrie book;
    clock_t start = clock();
    while(!myFile.eof())
    {
        myFile >> tempWord;
        if(tempWord == "----------------------------------------")
        {
            pageCount++;
            wordCount = 1;
        }
        else
        {
            c = tempWord[0];
            if(c < 90 && c > 63)
            {
                c = c + 32;
            }
            tempWord = c + tempWord.substr(1, tempWord.size() - 1);
            book.addWord(tempWord);
            wordCount++;
        }
    }
    clock_t duration = clock() - start;
    myFile.close();

    cout << "Time taken to build index: " << duration / (double)CLOCKS_PER_SEC << " seconds." << endl;
    cout << "Number of unique keys in index: " << book.getNumberUniqueWords() << endl;
    cout << "Number of total keys in index: " << book.getNumberTotalWords() << endl;

    cout << "\n----------------Trie Test----------------" << endl;
    cout << "Select test method:\n";
    cout << "1 - Print all\n";
    cout << "2 - Print all with prefix\n";
    cout << "3 - Search for word in trie\n";
    cin >> choice;
    cout << endl;
    switch(choice){
        case 0:
            break;
        case 1:
            book.printAll();
            break;
        case 2:
            cout << "Enter prefix: ";
            cin >> tempWord;
            book.printAllWithPrefix(tempWord);
            break;
        case 3:
            cout << "Enter word to search for: ";
            cin >> tempWord;
            if(book.contains(tempWord)){
                std::cout << tempWord << " is in the trie\n";
            }
            else{
                std::cout << tempWord << " is not in the trie\n";
            }
            break;
        case 4:
            cout << "Results written to file...\n";
            break;
        case 5:
            break;
        default:
            cout << "Invalid choice!\n";
            break;
    }
}

void BinaryTreeTest(){

    BinaryTree<int> myBinaryTree;
    int choice = -1;
    int tree_input = 0;

    while(choice != 0)
    {
        cout << "\n----------------Binary Tree Test----------------" << endl;
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

