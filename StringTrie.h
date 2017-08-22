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

#ifndef STRINGTRIE_H
#define STRINGTRIE_H
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <map>

struct StringTrieNode{
    bool is_a_word = false;
    int number_of_times_seen = 0;
    char data;
    StringTrieNode* parent = nullptr;
    StringTrieNode** paths = new StringTrieNode*[200]();
    StringTrieNode(char input_char){
        data = input_char;
    }

    // BUG : currently not deallocating memory correctly
    ~StringTrieNode(){
        std::vector<StringTrieNode*> all_paths;
        StringTrieNode* current;
        all_paths.push_back(this);
        while(all_paths.size() > 0){
            current = all_paths.back();
            all_paths.pop_back();
            for(int i = 0; i < 200; i++){
                if(current->paths[i] != nullptr){
                    all_paths.push_back(current->paths[i]);
                }
            }
            delete current->paths;
            delete current;
        }
    }
};

class StringTrie
{
public:
    StringTrie();
    StringTrie(StringTrie &other_trie);
    ~StringTrie();
    void addWord(std::string word);
    bool contains(std::string word);
    void remove(std::string word);
    void removeSubtrie(std::string word);
    int getNumberTotalWords() const;
    int getNumberUniqueWords() const;
    int getLengthOfShortestWord() const;
    int getLengthOfLongestWord() const;
    void printAll();
    void printAllWithPrefix(std::string word);
    void printAllHelper(StringTrieNode* current, std::string word);
    std::map<int, StringTrieNode*> m_paths;
    std::hash<char> m_hasher;

private:
    StringTrieNode* head;

    int number_of_total_words = 0;
    int number_of_unique_words = 0;
    int alphabet_size = 200;
};


StringTrie::StringTrie(){
    head = new StringTrieNode('\0');
}

StringTrie::~StringTrie(){
    delete head;
}

void StringTrie::addWord(std::string word){
    StringTrieNode** current_list = head->paths;
    StringTrieNode* current_node = head->paths[0];
    const char* c_string_word = word.c_str();
    int index_of_char = -1;
    int size = word.length();
    for(int i = 0; i < size; i++){
         char t_char = c_string_word[i];
         //index_of_char = m_hasher(c_string_word[i]);
         //if(m_paths[index_of_char] == nullptr){
         //   StringTrieNode* t_tree_node = new StringTrieNode(c_string_word[i]);
         //   std::pair<int, StringTrieNode*>  t_pair(index_of_char, t_tree_node);
         //  m_paths.insert(t_pair);
         //}
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
        index_of_char = tolower(c_string_word[i]);
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

void StringTrie::remove(std::string word){
    StringTrieNode** current_list = head->paths;
    StringTrieNode* current_node = head->paths[0];
    std::vector<StringTrieNode*> node_stack;
    const char* c_string_word = word.c_str();
    int index_of_char = -1;
    int size = word.length();
    for(int i = 0; i < size; i++){
        index_of_char = tolower(c_string_word[i]);
        if(current_list[index_of_char] == nullptr)
        {
            return;
        }
        node_stack.push_back(current_node);
        current_node = current_list[index_of_char];
        current_list = current_list[index_of_char]->paths;
    }
    if(current_node->is_a_word){
        current_node->is_a_word = false;
        bool delete_node = true;
        while (node_stack.size() > 0 && delete_node == true){
            for(int i = 0; i < alphabet_size; i++){
                if(current_node->paths[i] != nullptr){
                    delete_node = false;
                }
            }
            if(delete_node == true)
            {
                delete current_node;
            }
            current_node = node_stack.back();
            node_stack.pop_back();
        }
    }
}

void StringTrie::removeSubtrie(std::string word){
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
    delete current_node;
}

void StringTrie::printAll(){
    printAllHelper(head, "");
}

// prints all words with a given prefix
// input: prefix of words to print
void StringTrie::printAllWithPrefix(std::string word){
    StringTrieNode** current_list = head->paths;
    StringTrieNode* current_node = head->paths[0];
    const char* c_string_word = word.c_str();
    int index_of_char = -1;
    int size = word.length();
    for(int i = 0; i < size; i++){
        index_of_char = tolower(c_string_word[i]) - 97;
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

// prints all words in subtree, building up word one character at a time with each rescursive call
void StringTrie::printAllHelper(StringTrieNode* current, std::string word){
    if(current->is_a_word){
        std::cout << word + current->data << std::endl;
    }
    for(int i = 0; i < alphabet_size; i++){
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
    StringTrieNode* current_node;
    int min_length = 1;
    int current_level_paths_left = 1;
    int next_level_paths = 0;
    possible_words->push(head);
    while (possible_words->size() > 0){
        current_node = possible_words->front();
        possible_words->pop();
        for(int i = 0; i < alphabet_size; i++){
            if(current_node->paths[i] != nullptr){
                if(current_node->paths[i]->is_a_word){
                    return min_length;
                }
                next_level_paths++;
                possible_words->push(current_node->paths[i]);
            }
        }
        current_level_paths_left--;
        if(current_level_paths_left == 0){
            min_length++;
            current_level_paths_left = next_level_paths;
            next_level_paths = 0;
        }
    }
    return 0;
}

#endif // STRINGTRIE_H
