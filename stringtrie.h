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

class StringRecord;

struct StringTrieNode {
    bool is_a_word;
    const char data;
    StringTrieNode* parent;
    // hash table containing points to all suffixes
    std::map<char, StringTrieNode*> m_paths;
    // creates node containing input_char
    explicit StringTrieNode(char input_char);
    // deletes node and all children nodes in subtrie
    ~StringTrieNode();
};

class StringTrie {
 public:
    // constructs empty trie
    StringTrie();
    // creates deep copy of trie
    explicit StringTrie(const StringTrie &other_trie);
    // destructor
    // calls removeSubTrie(head)
    ~StringTrie();

    // adds word to trie
    void addWord(const std::string &word);

    // removes word from trie, deleting any node that isn't the prefix
    // of another another word
    void remove(const std::string &word);

    // removes all words with a given prefix
    // input:
    //    string prefix - prefix to search for
    void removeAllWithPrefix(const std::string &prefix);

    // returns true if word is in the trie
    bool contains(const std::string &word);

    // returns total number of words within trie
    int getNumberTotalWords() const;
    // returns number of unique words within the trie
    int getNumberUniqueWords() const;

    std::string getLongestWord() const;

    int getLengthOfShortestWord() const;
    int getLengthOfLongestWord() const;

    void printAll();
    void printAllWithPrefix(const std::string &prefix);

    friend class StringRecord;
    StringRecord* m_record;

 protected:
    // constructs word character by character starting from the
    // last character in the word
    void buildStringFromFinalNode(StringTrieNode* current_node,
                                  std::string &word) const ;

 private:
    // prints all words in subtree,
    // word is built up one character at a time with each rescursive call
    void printAllHelper(StringTrieNode* current, std::string word);

    // recursive function that finds the longest suffix from the current node
    void findLongestWord(StringTrieNode* current_node, int current_length,
                         int &longest_length, StringTrieNode *&longest) const;

    // helper function that deletes all nodes in subtrie
    void removeSubTrie(StringTrieNode* current);

    StringTrieNode* head;
    int number_of_total_words = 0;
    int number_of_unique_words = 0;
};

class StringRecord {
 public:
    StringRecord() {}
    inline const std::map<StringTrieNode*, int>* getRecord() { return &m_record; }
    void addWord(StringTrieNode* current_node);
    std::vector<std::pair<std::string, int>> getOrderedWords(const StringTrie *trie);
    // std::vector<int> getOrderedOccurances;
 private:
    std::map<StringTrieNode*, int> m_record;
};

#endif  // STRINGTRIE_H_
