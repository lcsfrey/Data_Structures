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
#include <string>

class StringRecord;

class StringTrieNode {
 public:
  // creates node containing input_char
  explicit StringTrieNode(const char &input_char);
  // deletes node and all children nodes in subtrie
  ~StringTrieNode();

  // returns true if input char is a valid suffix char
  // (e.g. input char is a key value in m_paths)
  inline bool hasSuffixNode(const char &input_char) const {
    return m_paths.find(input_char) != m_paths.end();
  }

  // returns pointer to suffix node node if it exists
  // returns nullptr if it does not exist
  inline StringTrieNode* getSuffixNode(const char &input_char) {
      return hasSuffixNode(input_char) ? m_paths[input_char] : nullptr;
  }

  friend class StringTrie;

 protected:
  const char data;
  bool is_a_word;
  StringTrieNode* parent;
  // hash table containing pointers to all suffixes
  std::map<char, StringTrieNode*> m_paths;
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

  void resetTrie();

  // adds word to trie
  void addWord(const std::string &word);

  // removes word from trie, deleting any node that isn't the prefix
  // of another another word
  void remove(const std::string &word);

  // removes all words with a given prefix
  // input:
  //  string prefix - prefix to search for
  void removeAllWithPrefix(const std::string &prefix);

  // returns true if word is in the trie
  bool contains(const std::string &word);

  // returns total number of words within trie
  int getNumberTotalWords() const;

  // returns number of unique words within the trie
  int getNumberUniqueWords() const;

  int getNumberOccurences(const std::string &word);

  std::string getLongestWord() const;

  int getLengthOfShortestWord() const;
  int getLengthOfLongestWord() const;

  void printAll() const;
  void printAllWithPrefix(const std::string &prefix) const;

  // prints all strings in trie ordered by the number of times that string has
  // been added to the trie
  void printAllByOccurences() const;

  // prints strings in trie ordered by the number of times that string has been
  // added to the trie. Only prints strings that have with a frequency less
  // than or equal to upper_limit and greater than or equal to lower_limit
  void printOccurencesInRange(int upper_limit = INT32_MAX,
                              int lower_limit = 0) const;

  // prints the most common words in the trie. Only prints a number of strings
  // equal to limit
  void printTopOccurences(int limit = 1000);

  void writeToFile(std::string filename = "trieFile.txt") const;

  void readFromFile(std::string filename = "trieFile.txt");

  friend class StringRecord;
  friend class StringSequenceTrie;
  friend class StringSequenceTrieNode;
  StringRecord* m_record;

 protected:
  // constructs word character by character starting from the
  // last character in the word
  std::string buildStringFromFinalNode(const StringTrieNode *current_node) const;

  // returns StringTrieNode pointer pointing to the final node
  // corresponding to the last node (character) of the string
  // returns nullptr if word is not in trie
  StringTrieNode* getNode(const std::string &word);

 private:
  // prints all words in subtree,
  // word is built up one character at a time with each rescursive call
  void printAllHelper(StringTrieNode* current, std::string word) const;

  // recursive function that finds the longest suffix from the current node
  void findLongestWord(StringTrieNode* current_node, int current_length,
    int &longest_length, StringTrieNode *&longest) const;

  // helper function that deletes all nodes in subtrie
  void removeSubTrie(StringTrieNode* current);

  void readFromFileHelper(std::ifstream &infile,
                          StringTrieNode *current_node) const;

  void writeToFileHelper(std::ofstream &outfile,
                         const StringTrieNode *current_node) const;

  StringTrieNode* head;
  int number_of_total_words = 0;
  int number_of_unique_words = 0;
};

class StringRecord {
 public:
  StringRecord() {}

  // add final node (character) of word to map
  // or increment value (occurences) if already in map
  void addWord(const StringTrieNode* current_node, int occurences = 1);

  void removeWord(const StringTrieNode* current_node);

  int getNumberOccurences(const StringTrieNode *current_node) const;

  // returns ordered list of occurences
  std::vector<int> getOrderedOccurences() const;

  // returns map of associating StringTrieNode pointers and their frequency
  inline const std::unordered_map<const StringTrieNode*, int>* getRecord() {
    return &m_record; }

  // returns ordered list of strings and their occurences
  std::vector<std::pair<std::string, int>> getOrderedWords(
      const StringTrie *trie,
      const int upper_limit = INT32_MAX,
      const int lower_limit = 0);

 protected:
  // map of key containing the final node (character) of
  // the word and the number of times it has been seen
  std::unordered_map<const StringTrieNode*, int> m_record;
};


#endif  // STRINGTRIE_H_
