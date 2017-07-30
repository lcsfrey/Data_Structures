#ifndef STRINGTRIE_H
#define STRINGTRIE_H
#include <string>
#include <iostream>
#include <Queue>
#include "LinkedList.h"

//Dependecies: LinkedList.h from https://github.com/lcsfrey/Data_Structures

struct StringTrieNode : ListNode<char>{
    bool is_a_word;
    int number_of_times_seen = 0;
    StringTrieNode** paths = new StringTrieNode*[200]();
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
    int getNumberTotalWords() const;
    int getNumberUniqueWords() const;
    int getLengthOfShortestWord() const;
    int getLengthOfLongestWord() const;
    void printAll();
    void printAllWithPrefix(std::string word);
    void printAllHelper(StringTrieNode* current, std::string word);

private:
    StringTrieNode* head;
    int number_of_total_words = 0;
    int number_of_unique_words = 0;
};


StringTrie::StringTrie(){
    head = new StringTrieNode('\0');
}

void StringTrie::addWord(std::string word){
    StringTrieNode** current_list = head->paths;
    StringTrieNode* current_node = head->paths[0];
    const char* c_string_word = word.c_str();
    int index_of_char = -1;
    int size = word.length();
    for(int i = 0; i < size; i++){
        index_of_char = tolower(c_string_word[i]);
        if(current_list[index_of_char] == nullptr){
            current_list[index_of_char] = new StringTrieNode(c_string_word[i]);
        }
        current_node = current_list[index_of_char];
        current_list = current_list[index_of_char]->paths;
    }
    if(!(current_node->is_a_word)){
        number_of_unique_words +=1;
    }
    current_node->is_a_word = true;
    number_of_total_words += 1;
}

bool StringTrie::contains(std::string word){
    StringTrieNode** current_list = head->paths;
    StringTrieNode* current_node = head->paths[0];
    const char* c_string_word = word.c_str();
    int index_of_char = -1;
    int size = word.length();
    for(int i = 0; i < size; i++){
        index_of_char = tolower(c_string_word[i]) - 97;
        if(current_list[index_of_char] == nullptr)
        {
            return false;
        }
        current_node = current_list[index_of_char];
        current_list = current_list[index_of_char]->paths;
    }
    if(current_node->is_a_word){
        return true;
    }
    else{
        return false;
    }
}

void StringTrie::printAll(){
    printAllHelper(head, "");
}

void StringTrie::printAllWithPrefix( std::string word){
    StringTrieNode** current_list = head->paths;
    StringTrieNode* current_node = head->paths[0];
    const char* c_string_word = word.c_str();
    int index_of_char = -1;
    int size = word.length();
    for(int i = 0; i < size; i++){
        index_of_char = tolower(c_string_word[i]);
        if(current_list[index_of_char] == nullptr)
        {
            std::cout << "\nNo words with prefix: " << word << std::endl;
            return;
        }
        current_node = current_list[index_of_char];
        current_list = current_list[index_of_char]->paths;
    }
    printAllHelper(current_node, word.substr(0, word.size()-1));
}

void StringTrie::printAllHelper(StringTrieNode* current, std::string word){
    if(current->is_a_word){
        std::cout << word + current->data << std::endl;
    }
    for(int i = 0; i < 200; i++){
        if(current->paths[i] != nullptr){
            printAllHelper(current->paths[i], word + current->data);
        }
    }
}

int StringTrie::getNumberTotalWords() const{
    return number_of_total_words;
}

int StringTrie::getNumberUniqueWords() const{
    return number_of_unique_words;
}

// returns the length of the shortest word in trie
int StringTrie::getLengthOfShortestWord() const{
    std::queue<StringTrieNode*>* possible_words = new std::queue<StringTrieNode*>();
    std::queue<int>* possible_words_paths = new std::queue<int>();
    StringTrieNode* current_node;
    int min_length = 0;
    int current_node_possible_paths = 0;
    int current_node_lenth = 0;
    int words_left = 0;
    possible_words->push(head);
    while (possible_words->size() > 0){
        current_node = possible_words->front();
        possible_words->pop();
        words_left = possible_words_paths->front();
        possible_words_paths->pop();

        current_node_possible_paths = 0;
        for(int i = 0; i < 200; i++){
            if(current_node[i].is_a_word){
                return min_length;
            }
            if(current_node->paths[i] != nullptr){
                current_node_possible_paths++;
                possible_words_paths->push(current_node_lenth);
                possible_words->push(current_node->paths[i]);
            }
        }
    }
}

#endif // STRINGTRIE_H
