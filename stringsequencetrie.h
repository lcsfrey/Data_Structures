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

    // returns a sequence of strings one string at a time from the last node;
    std::string buildSequenceFromFinalNode(const StringSequenceTrieNode *current) const;

    void loadTextFile(std::string file_name = "");

 protected:
    // returns a node pointing to the last node in the sequence
    StringSequenceTrieNode* getNode(const std::string &sequence) const;


    void addSequenceHelper(const std::string &sequence, int starting_pos = 0,
                           bool add_word_to_trie = true);

 private:
    // contains dictionary of words that have been used
    StringTrie* m_trie;

    StringSequenceTrieNode* head;

    int m_total_words;
};
#endif // STRINGSEQUENCETRIE_H
