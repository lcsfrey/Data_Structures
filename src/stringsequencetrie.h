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

#ifndef STRINGSEQUENCETRIE_H_
#define STRINGSEQUENCETRIE_H_

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>
#include <string>

#include "stringtrie.h"

class StringSequenceTrieNode {
 public:
  StringSequenceTrieNode(StringTrieNode* string_trie_node,
                         StringSequenceTrieNode *parent);

  bool containsNextWord(StringTrieNode* word) const {
    return m_next_word.find(word) != m_next_word.end();
  }

  void addChild(StringTrieNode* word) {
    m_next_word[word] = new StringSequenceTrieNode(word, this);
    addTimesSeen(1);
  }

  void addTimesSeen(const int occurences) { m_times_seen += occurences; }

  int getTimesSeen() const { return m_times_seen; }

  const StringTrieNode* getWordNode() const { return m_trie_word_node; }

  const StringSequenceTrieNode* getNextSequenceNode(StringTrieNode* next_word) {
      return containsNextWord(next_word) ? m_next_word[next_word] : nullptr;
  }

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


class StringSequenceTrie {
 public:

  StringSequenceTrie();

  class SequenceCriteria {
   public:
    SequenceCriteria(){}
    void setStartingSequence(std::string starting_sequence);

    std::string m_starting_sequence = "";
    // std::unordered_set<std::string> m_black_list_words;
    int m_length_max_count = 3;
    int m_length_min_count = 2;
    int m_branching_factor = 50;
    int m_frequency_max = INT32_MAX;
    int m_frequency_min = 3;
  };

  // add sequence of strings separated by spaces to trie
  void addSequence(const std::string &sequence);

  // add sequence of strings from vector to trie
  void addSequence(const std::vector<std::string> &sequence,
                   int window_size = 5);

  std::string getNextWord(std::string &sequence);

  // returns a vector of StringSequenceTrieNode pointers ordered by the number
  // of times that node (the sequence ending with the word contained in that
  // node has been seen.
  // seqence_length_upper_limit - max number of words in the sequence
  // seqence_length_lower_limit - minimum number of words in the sequence
  // branching_factor - number of possible next words following the current word
  std::vector<StringSequenceTrieNode*> getOrderedWords(
      const SequenceCriteria &criteria) const;

  void printOrderedWords(const SequenceCriteria &criteria) const;

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
      StringSequenceTrieNode *current_seq_node, std::size_t starting_pos = 0);

  void addSequenceBackwardHelper(const std::string &sequence,
      StringSequenceTrieNode *current_seq_node, std::size_t starting_pos = 0);

  void getOrderedWordsHelper(const StringSequenceTrieNode* current_node,
      std::vector<StringSequenceTrieNode *> *sequences,
      const SequenceCriteria &criteria, int current_sequence_length) const;

 private:
  void readFromFileHelper(std::ifstream &infile,
      StringSequenceTrieNode *current_seq_node);

  void writeToFileHelper(std::ofstream &outfile,
      const StringSequenceTrieNode *current_node) const;

  StringTrie* m_trie;

  StringSequenceTrieNode* m_seq_head;

  StringSequenceTrieNode* m_seq_backward_head;

  int m_total_words;

  int m_window_size;
};


#endif  // STRINGSEQUENCETRIE_H_
