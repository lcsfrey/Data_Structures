#ifndef STRINGTRIE_H
#define STRINGTRIE_H
#include <string>
#include <iostream>
#include "LinkedList.h"

//Dependecies: LinkedList.h from https://github.com/lcsfrey/Data_Structures

struct StringTrieNode : ListNode<char>{
    bool is_a_word;
    StringTrieNode** paths = new StringTrieNode*[26]();
    StringTrieNode(char input_char){
        is_a_word = false;
        data = input_char;
    }
};

class StringTrie
{
public:
    StringTrie();
    void addWord(std::string word);
    bool contains(std::string word);
    void printAll();
    void printAllHelper(StringTrieNode* current, std::string word);

private:
    StringTrieNode* head;
};


#endif // STRINGTRIE_H
