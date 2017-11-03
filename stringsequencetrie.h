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


#include "stringtrie.h"


class StringSequenceTrieNode {
 public:
    StringSequenceTrieNode(StringTrieNode* string_trie_node,
                           StringSequenceTrieNode* parent);

    inline void addOneTimeSeen() { m_times_seen += 1; }

    const StringTrieNode* getWordNode();

    inline int getTimesSeen() { return m_times_seen; }

    std::string getWord();

    // current word in the sequence
    const StringTrieNode* m_trie_word_node;

    // pointer to previous word in sequence
    const StringSequenceTrieNode* m_parent;

    friend class StringSequenceTrie;

  protected:
    std::unordered_map<StringTrieNode*, StringSequenceTrieNode*> m_next_word;

    int m_times_seen;
};

class StringSequenceTrie {
 public:
    StringSequenceTrie();

    // add sequence of strings separated by spaces to record
    void addSequence(const std::string &sequence);

    // add sequence of strings from vector to record
    void addSequence(std::vector<std::string> &sequence, int window_size = 5);

    std::vector<StringSequenceTrieNode*> getOrderedWords(const std::string sequence = "",
                                                         int sequence_length_upper_limit = INT32_MAX,
                                                         int sequence_length_lower_limit = 1,
                                                         int branching_factor = 5);

    void printOrderedWords(const std::string sequence = "",
                           int sequence_length_upper_limit = INT32_MAX,
                           int sequence_length_lower_limit = 1,
                           int branching_factor = 5,
                           int frequency_upper_limit = INT32_MAX,
                           int frequency_lower_limit = 0);

    // returns a sequence of strings one string at a time from the last node;
    std::string buildSequenceFromFinalNode(const StringSequenceTrieNode *current) const;

    void loadTextFile(std::string file_name = "", int window_size = 5);

 protected:
    // returns a node pointing to the last node in the sequence
    StringSequenceTrieNode* getNode(const std::string &sequence) const;


    void addSequenceHelper(const std::string &sequence, StringSequenceTrieNode *current_node,
                           int starting_pos = 0);


    void getOrderedWordsHelper(const StringSequenceTrieNode* current_node,
                               std::vector<StringSequenceTrieNode *> &sequences,
                               const int &sequence_length_upper_limit = INT32_MAX,
                               const int &sequence_length_lower_limit = 1,
                               int current_sequence_length = 0,
                               int branching_factor = 5);

 private:
    // contains dictionary of words that have been used
    StringTrie* m_trie;

    StringSequenceTrieNode* head;

    int m_total_words;
};


#endif // STRINGSEQUENCETRIE_H
