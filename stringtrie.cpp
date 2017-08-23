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

#include "StringTrie.h"

#include <vector>
#include <queue>
#include <map>
#include <utility>
#include <string>

StringTrieNode::~StringTrieNode() {
    if (this->m_paths.size() == 0) {
        return;
    }
    std::queue<StringTrieNode*> all_paths;
    StringTrieNode* current = this;
    all_paths.push(this);
    while (all_paths.size() > 0) {
        current = all_paths.front();
        all_paths.pop();
        for (auto iter : current->m_paths) {
            all_paths.push(iter.second);
        }
        delete current;
    }
}

StringTrie::StringTrie() {
    head = new StringTrieNode('\0');
}

StringTrie::~StringTrie() {
    delete head;
}

void StringTrie::addWord(const std::string &word) {
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    StringTrieNode* current_node = head;
    const char* c_string_word = word.c_str();
    int size = word.length();
    for (int i = 0; i < size; i++) {
        char key_char = tolower(c_string_word[i]);
        std::map<char, StringTrieNode*>::const_iterator found(current_map->find(key_char));
        if (found == current_map->end()) {
            StringTrieNode* t_node_pointer = new StringTrieNode(key_char);
            std::pair<char, StringTrieNode*> t_pair(key_char, t_node_pointer);
            current_map->insert(t_pair);
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }

    if (!current_node->is_a_word) number_of_unique_words +=1;

    current_node->is_a_word = true;
    number_of_total_words += 1;
}

bool StringTrie::contains(const std::string &word) {
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    StringTrieNode* current_node = head;
    const char* c_string_word = word.c_str();
    char key_char = -1;
    int size = word.length();
    for (int i = 0; i < size; i++) {
        key_char = tolower(c_string_word[i]);
        if (current_map->find(key_char) == current_map->end()) {
            return false;
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }
    if (current_node->is_a_word) {
        return true;
    } else {
        return false;
    }
}

void StringTrie::remove(const std::string &word) {
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    std::map<char, StringTrieNode*>* parent_map = &head->m_paths;
    std::vector<StringTrieNode*> potential_deletions;
    StringTrieNode* current_node = head;
    int size = word.size();
    const char* c_string_word = word.c_str();
    char key_char = -1;
    for (int i = 0; i < size; i++) {
        potential_deletions.push_back(current_node);
        key_char = tolower(c_string_word[i]);
        if (current_map->find(key_char) == current_map->end()) {
            printf("%s is not a word in the trie\n", c_string_word);
            return;
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }
    potential_deletions.push_back(current_node);

    while (potential_deletions.size() > 1) {
        current_node = potential_deletions.back();
        potential_deletions.pop_back();
        parent_map = &potential_deletions.back()->m_paths;

        if (current_node->m_paths.size() == 0) {
            parent_map->at(key_char) = nullptr;
            delete current_node;
        } else {
            current_node->is_a_word = false;
            return;
        }
    }
}

void StringTrie::removeSubtrie(const std::string &word) {
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    StringTrieNode* current_node = head;
    const char* c_string_word = word.c_str();
    char key_char = -1;
    int size = word.length();
    for (int i = 0; i < size; i++) {
        key_char = tolower(c_string_word[i]);
        if (current_map->find(key_char) == current_map->end()) {
            std::cout << "\nNo words with prefix: " << word << std::endl;
            return;
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }
    current_node->m_paths.clear();
}

void StringTrie::printAll() {
    printAllHelper(head, "");
}

// prints all words with a given prefix
// input: prefix of words to print
void StringTrie::printAllWithPrefix(const std::string &word) {
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    StringTrieNode* current_node = head;
    const char* c_string_word = word.c_str();
    char key_char = -1;
    int size = word.length();
    for (int i = 0; i < size; i++) {
        key_char = tolower(c_string_word[i]);
        if (current_map->find(key_char) == current_map->end()) {
            std::cout << "\nNo words with prefix: " << word << std::endl;
            return;
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }
    printAllHelper(current_node, word);
}

// prints all words in subtree,
// word is built up one character at a time with each rescursive call
void StringTrie::printAllHelper(StringTrieNode* current, std::string word) {
    if (current->is_a_word) {
        std::cout << word + current->data << std::endl;
    }
    for (auto iter : current->m_paths) {
        printAllHelper(iter.second, word + iter.first);
    }
}

int StringTrie::getNumberTotalWords() const {
    return number_of_total_words;
}

int StringTrie::getNumberUniqueWords() const {
    return number_of_unique_words;
}

// returns the length of the shortest word in trie
int StringTrie::getLengthOfShortestWord() const {
    std::queue<StringTrieNode*> possible_words;
    StringTrieNode* current_node;
    int min_length = 1;
    int current_level_paths_left = 1;
    int next_level_paths = 0;
    possible_words.push(head);
    while (possible_words.size() > 0) {
        current_node = possible_words.front();
        possible_words.pop();
        for (auto iter : current_node->m_paths) {
            if (iter.second->is_a_word) {
                return min_length;
            }
            next_level_paths++;
            possible_words.push(iter.second);
        }
        current_level_paths_left--;
        if (current_level_paths_left == 0) {
            min_length++;
            current_level_paths_left = next_level_paths;
            next_level_paths = 0;
        }
    }
    return 0;
}
