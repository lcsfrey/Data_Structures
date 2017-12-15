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

#ifndef STRINGSEQUENCETRIE_H
#define STRINGSEQUENCETRIE_H

#include <unordered_map>
#include <vector>

#include "stringtrie.h"

class StringSequenceTrieNode {
 public:
  StringSequenceTrieNode(StringTrieNode* string_trie_node,
                         StringSequenceTrieNode* parent);

  inline bool containsNextWord(StringTrieNode* word) const {
    return m_next_word.find(word) != m_next_word.end();
  }

  inline void addChild(StringTrieNode* word) {
    m_next_word[word] = new StringSequenceTrieNode(word, this);
  }
  inline void addOneTimeSeen() { m_times_seen += 1; }

  inline int getTimesSeen() const { return m_times_seen; }

  inline const StringTrieNode* getWordNode() const { return m_trie_word_node; }

  friend class StringSequenceTrie;

  protected:
  // current word in the sequence
  const StringTrieNode* m_trie_word_node;

  // pointer to previous word in sequence
  const StringSequenceTrieNode* m_parent;

  // hash table of all possible next words
  std::unordered_map<StringTrieNode*, StringSequenceTrieNode*> m_next_word;

  // number of times the sequence has been seen
  int m_times_seen;
};


class StringSequenceTrie : StringTrie {
 public:
  StringSequenceTrie();

  // add sequence of strings separated by spaces to trie
  void addSequence(const std::string &sequence);

  // add sequence of strings from vector to trie
  void addSequence(std::vector<std::string> &sequence, int window_size = 5);

  // returns a vector of StringSequenceTrieNode pointers ordered by the number
  // of times that node (the sequence ending with the word contained in that node)
  // has been seen.
  // The function can be restricted to only return nodes within a range of lengths.
  // A branching factor can be specified which affects how many unique words at
  // each branch will be checked.
  std::vector<StringSequenceTrieNode*> getOrderedWords(
      const std::string sequence = "",
      int sequence_length_upper_limit = INT32_MAX,
      int sequence_length_lower_limit = 1,
      int branching_factor = 5) const;

  void printOrderedWords(const std::string sequence = "",
      int sequence_length_upper_limit = INT32_MAX,
      int sequence_length_lower_limit = 1,
      int branching_factor = 5,
      int frequency_upper_limit = INT32_MAX,
      int frequency_lower_limit = 0) const;

  void printMostFrequentSequences(int limit = 1000) const;

  // returns a sequence of strings one string at a time from the last node;
  std::string buildSequenceFromFinalNode(const StringSequenceTrieNode* current) const;

  void writeToFile(std::string filename = "trieFile.txt") const;

  void readFromFile(std::string filename = "trieFile.txt");

  void loadTextFile(std::string file_name = "", int window_size = 5);

 protected:
  // returns a node pointing to the last node in the sequence
  StringSequenceTrieNode* getNode(const std::string &sequence) const;

  void addSequenceHelper(const std::string &sequence,
      StringSequenceTrieNode *current_node,
      int starting_pos = 0);

  void getOrderedWordsHelper(const StringSequenceTrieNode* current_node,
      std::vector<StringSequenceTrieNode *> &sequences,
      const int &sequence_length_upper_limit = INT32_MAX,
      const int &sequence_length_lower_limit = 1,
      int current_sequence_length = 0,
      int branching_factor = 5) const;

 private:

  void readFromFileHelper(std::ifstream &infile,
      StringSequenceTrieNode *current_node);

  void writeToFileHelper(std::ofstream &outfile,
      const StringSequenceTrieNode *current_node) const;

  // contains dictionary of words that have been used
  StringTrie* m_trie;

  StringSequenceTrieNode* head;

  int m_total_words;
};


#endif // STRINGSEQUENCETRIE_H
