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

#ifndef STRINGTRIE_H_
#define STRINGTRIE_H_
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <utility>

struct StringTrieNode {
    explicit StringTrieNode(char input_char) : data(input_char) {}

    // BUG: currently not deallocating memory correctly
    ~StringTrieNode();
    bool is_a_word = false;
    char data;
    StringTrieNode* parent = nullptr;
    std::map<char, StringTrieNode*> m_paths;
};

class StringTrie {
 public:
    StringTrie();
    explicit StringTrie(const StringTrie &other_trie);
    ~StringTrie();

    void addWord(const std::string &word);

    void remove(const std::string &word);
    void removeSubtrie(const std::string &word);

    bool contains(const std::string &word);

    int getNumberTotalWords() const;
    int getNumberUniqueWords() const;
    int getLengthOfShortestWord() const;
    int getLengthOfLongestWord() const;

    void printAll();
    void printAllWithPrefix(const std::string &word);
    void printAllHelper(StringTrieNode* current, std::string word);

 private:
    StringTrieNode* head;
    int number_of_total_words = 0;
    int number_of_unique_words = 0;
};


#endif  // STRINGTRIE_H_
