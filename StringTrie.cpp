#include "StringTrie.h"

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
        index_of_char = tolower(c_string_word[i]) - 97;
        if(current_list[index_of_char] == nullptr){
            current_list[index_of_char] = new StringTrieNode(c_string_word[i]);
        }
        current_node = current_list[index_of_char];
        current_list = current_list[index_of_char]->paths;
    }
    current_node->is_a_word = true;
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

void StringTrie::printAllHelper(StringTrieNode* current, std::string word){
    if(current->is_a_word){
        std::cout << word + current->data << std::endl;
    }
    for(int i = 0; i < 26; i++){
        if(current->paths[i] != nullptr){
            printAllHelper(current->paths[i], word + current->data);
        }
    }
}

