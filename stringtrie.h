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
#include <unordered_map>
#include <map>
#include <utility>

class StringRecord;

struct StringTrieNode {
    bool is_a_word;
    const char data;
    StringTrieNode* parent;
    // hash table containing pointers to all suffixes
    std::map<char, StringTrieNode*> m_paths;
    // creates node containing input_char
    explicit StringTrieNode(char input_char);

    bool operator ==(const StringTrieNode* other_node) {
      return parent == other_node->parent;
    }

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
    friend class StringSequenceRecord;
    StringRecord* m_record;

 protected:
    // constructs word character by character starting from the
    // last character in the word
    void buildStringFromFinalNode(StringTrieNode* current_node,
                                  std::string &word) const;

    inline StringTrieNode* getNode(const std::string &word);


 private:
    // prints all words in subtree,
    // word is built up one character at a time with each rescursive call
    void printAllHelper(StringTrieNode* current, std::string word) const;

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
    // constructs a String Record with window_size of related 
    // words in front of and behind it
    StringRecord() { std::cout << "Initializing empty StringRecord\n"; }

    // add final node (character) of word to map
    // or increment value (occurences) if already in map
    void addWord(StringTrieNode* current_node);
    
    // add string and their most likely next and previous strings
    void addWord(StringTrieNode* current_node, 
                 std::vector<StringTrieNode*> sequence);

    // returns ordered list of occurences
    std::vector<int> getOrderedOccurences();
    
    // returns map of associating StringTrieNode pointers and their frequency
    inline const std::unordered_map<StringTrieNode*, int>* getRecord() { return &m_record; }
    
    // returns ordered list of strings and their occurences
    std::vector<std::pair<std::string, int>> getOrderedWords(const StringTrie *trie,
                                                             const int lower_limit = 10,
                                                             const int upper_limit = INT32_MAX);
    

 private:
    // map of key containing the final node (character) of
    // the word and the number of times it has been seen
    std::unordered_map<StringTrieNode*, int> m_record;
};

class StringSequenceRecord {
 public:
    StringSequenceRecord(int window_size = 5) : m_window_size(window_size) {
        m_total_words = 0;
        m_trie = new StringTrie();
        std::cout << "Constructing StringSequenceRecord\n";
    }

    // add sequence of strings to record
    void addSequence(const std::string &sequence);
    void addSequenceHelper(const std::string &sequence,
                     short starting_pos = 0,
                     short current_window_size = 5);

    void loadTextFile(std::string file_name = "");

 private:
    // defines the number of strings ahead of and behind the current string
     int m_window_size;

     // current word in the sequence
     const StringTrieNode* current_word;

     // next_word[StringRecord*] returns pair containing the next string
     // and the frequency of those words
     std::unordered_map<StringRecord*, std::pair<StringTrieNode*, int>> next_word;
     int m_total_words;

     // contains dictionary of words that have been used
     StringTrie* m_trie;
};

#endif  // STRINGTRIE_H_
