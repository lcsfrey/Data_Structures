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

#include "stringtrie.h"

#include <vector>
#include <queue>
#include <map>
#include <utility>
#include <string>

StringTrieNode::StringTrieNode(char input_char)
  : data(input_char),
    parent(nullptr),
    is_a_word(false) {}

StringTrieNode::~StringTrieNode() {
}

void StringTrie::removeSubTrie(StringTrieNode* current) {
    for (std::pair<char, StringTrieNode*> iter : current->m_paths) {
        removeSubTrie(iter.second);
    }
    delete current;
}

StringTrie::StringTrie() {
    head = new StringTrieNode('\0');
}

StringTrie::~StringTrie() {
    removeSubTrie(head);
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
            t_node_pointer->parent = current_node;
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

//
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

    while (potential_deletions.size() >= 1) {
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

void StringTrie::removeAllWithPrefix(const std::string &prefix) {
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    StringTrieNode* current_node = head;
    const char* c_string_prefix = prefix.c_str();
    char key_char = -1;
    int size = prefix.length();
    for (int i = 0; i < size; i++) {
        key_char = tolower(c_string_prefix[i]);
        if (current_map->find(key_char) == current_map->end()) {
            std::cout << "\nNo words with prefix: " << prefix << std::endl;
            return;
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }
    removeSubTrie(current_node);
}

void StringTrie::printAll() {
    printAllHelper(head, "");
}

// prints all words with a given prefix
// input: prefix of words to print
void StringTrie::printAllWithPrefix(const std::string &prefix) {
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    StringTrieNode* current_node = head;
    const char* c_string_prefix = prefix.c_str();
    char key_char = -1;
    int size = prefix.length();
    for (int i = 0; i < size; i++) {
        key_char = tolower(c_string_prefix[i]);
        if (current_map->find(key_char) == current_map->end()) {
            std::cout << "\nNo words with prefix: " << prefix << std::endl;
            return;
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }
    printAllHelper(current_node, prefix);
}

// prints all words in subtree,
// word is built up one character at a time with each rescursive call
void StringTrie::printAllHelper(StringTrieNode* current, std::string word) {
    if (current->is_a_word) {
        std::cout << word << std::endl;
    }
    for (auto t_pair : current->m_paths) {
        printAllHelper(t_pair.second, word + t_pair.first);
    }
}

int StringTrie::getNumberTotalWords() const {
    return number_of_total_words;
}

int StringTrie::getNumberUniqueWords() const {
    return number_of_unique_words;
}

std::string StringTrie::getLongestWord() const {
    int length = 0;
    std::string word = "";
    StringTrieNode* final_node = head;
    findLongestWord(head, 0, length, final_node);
    buildStringFromFinalNode(final_node, word);
    return word;
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

// constructs string character by character
void StringTrie::buildStringFromFinalNode(StringTrieNode* current_node,
                                          std::string &word) const {
    if (current_node->data != '\0') {
        buildStringFromFinalNode(current_node->parent, word);
        word += current_node->data;
    }
}

// returns longest word in trie
//
void StringTrie::findLongestWord(StringTrieNode* current_node, int current_length,
                                 int &longest_length, StringTrieNode *&longest) const {
    for (auto next_node : current_node->m_paths) {
        findLongestWord(next_node.second, current_length+1, longest_length, longest);
    }
    if (current_node->is_a_word && current_length > longest_length) {
        longest_length = current_length;
        longest = current_node;
    }
}

int StringTrie::getLengthOfLongestWord() const {
    int max_length = 0;
    StringTrieNode* final_node = head;
    std::string word = "";
    findLongestWord(head, 0, max_length, final_node);
    buildStringFromFinalNode(final_node, word);
    std::cout << word << std::endl;
    return max_length;
}

