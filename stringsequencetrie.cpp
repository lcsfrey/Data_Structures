
#include <ctime>
#include <fstream>
#include <string>

#include "stringsequencetrie.h"
#include "stringtrie.h"

StringSequenceTrieNode::StringSequenceTrieNode(StringTrieNode *string_trie_node,
                                               StringSequenceTrieNode *parent)  : m_trie_word_node(string_trie_node),
                                                                                  m_parent(parent), m_next_word(),
                                                                                  m_times_seen(1) {}

StringSequenceTrie::StringSequenceTrie() : m_trie(new StringTrie()), head(new StringSequenceTrieNode(m_trie->head, nullptr)),
                                           m_total_words(0) {}

void StringSequenceTrie::addSequence(const std::string &sequence) {
    addSequenceHelper(sequence, 0, true);
    // std::cout << "Adding to Sequence Trie: " << sequence << std::endl;
}

void StringSequenceTrie::addSequence(std::vector<std::string> &sequence, int window_size) {
    int size = sequence.size();

    StringTrieNode* current_trie_node = nullptr;
    StringSequenceTrieNode* current_sequence_node = head;
    StringSequenceTrieNode* parent = head;

    for (int i = 0; i < window_size; i++) {
        m_trie->addWord(sequence[i]);
    }

    for (int i = 0, j = window_size; i < size - window_size; i++, j++) {
        m_trie->addWord(sequence[j]);
        current_sequence_node = head;
        parent = head;
        current_trie_node = nullptr;
        for (int k = i; k < i + window_size; k++) {
            current_trie_node = m_trie->getNode(sequence[k]);
            if (current_sequence_node->m_next_word.find(current_trie_node) == current_sequence_node->m_next_word.end()) {
                current_sequence_node->m_next_word[current_trie_node] = new StringSequenceTrieNode(current_trie_node, parent);
                current_sequence_node = current_sequence_node->m_next_word[current_trie_node];
            } else {
                current_sequence_node = current_sequence_node->m_next_word[current_trie_node];
                current_sequence_node->addOneTimeSeen();
            }
        }
        parent = current_sequence_node;
    }
}

std::string StringSequenceTrie::buildSequenceFromFinalNode(const StringSequenceTrieNode *current) const {
    if (current != head) {
        return buildSequenceFromFinalNode(current->m_parent) + m_trie->buildStringFromFinalNode(current->m_trie_word_node);
    } else {
        return "";
    }
}

void StringSequenceTrie::writeToFile(std::string filename) const {
    std::ofstream outfile(filename);
    int branches = 0;
    branches = this->head->m_next_word.size();
    outfile << branches << std::endl;
    for(const auto starting_word : head->m_next_word) {
        writeToFileHelper(outfile, starting_word.second);
    }
}

void StringSequenceTrie::writeToFileHelper(std::ofstream &outfile,
                                           const StringSequenceTrieNode *current_node) const {
    int current_size = current_node->m_next_word.size();
    outfile << buildStringFromFinalNode(current_node->m_trie_word_node) << " "
            << current_node->m_times_seen << " "
            << current_size << " ";

    if (current_size != 0)
        for(const auto starting_word : head->m_next_word)
            writeToFileHelper(outfile, starting_word.second);
    else
        outfile << std::endl;
}

void StringSequenceTrie::addSequenceHelper(const std::string &sequence,
                                           int starting_pos,
                                           bool add_word_to_trie) {

    // verify we have a sequence to look at
    std::size_t size = sequence.size();

    std::size_t space_index = -1;

    std::string str = "";
    StringTrieNode* current_trie_node = nullptr;
    StringSequenceTrieNode* current_sequence_node = head;
    StringSequenceTrieNode* parent = head;
    space_index = sequence.find(" ", starting_pos);
    // check and verify there is a space in the string, else return
    if (space_index < size) {
        str = sequence.substr(starting_pos, space_index - starting_pos);
    } else {
        str = sequence.substr(starting_pos, size - starting_pos);
    }

    if (add_word_to_trie) {
        m_trie->addWord(str);
    }
    current_trie_node = m_trie->getNode(str);

    if (current_sequence_node->m_next_word.find(current_trie_node) == current_sequence_node->m_next_word.end()) {
        current_sequence_node->m_next_word[current_trie_node] = new StringSequenceTrieNode(current_trie_node, parent);
        current_sequence_node = current_sequence_node->m_next_word[current_trie_node];
    } else {
        current_sequence_node = current_sequence_node->m_next_word[current_trie_node];
        current_sequence_node->addOneTimeSeen();
    }
    parent = current_sequence_node;

    if (space_index >= size) return;
    // add sequence of words starting at current_pos
    addSequenceHelper(sequence, (int)space_index + 1, add_word_to_trie);
    add_word_to_trie = false;
    starting_pos = space_index + 1;
}

StringSequenceTrieNode* StringSequenceTrie::getNode(const std::string &sequence) const {
    // verify we have a sequence to look at
    std::size_t size = sequence.size();

    // find the next space in the sentence
    std::size_t space_index = sequence.find(" ");

    std::string str = "";
    StringTrieNode* current_trie_node = nullptr;
    int current_pos = 0;
    StringSequenceTrieNode* current_record_node = head;

    do {
        // check and verify there is a space in the string, else return
        if (space_index < size) {
            str = sequence.substr(current_pos, space_index - current_pos);
        } else {
            str = sequence.substr(current_pos, size - current_pos);
        }

        // check if the word is in m_trie
        // if it is not, then it wouldn't be in the sequence record
        current_trie_node = m_trie->getNode(str);
        if (current_trie_node == nullptr) {
            return nullptr;
        }

        if (current_record_node->m_next_word.find(current_trie_node) == current_record_node->m_next_word.end()) {
            return nullptr;
        } else {
            current_record_node = current_record_node->m_next_word[current_trie_node];
        }
    } while (space_index < size);
    return current_record_node;
}


void StringSequenceTrie::loadTextFile(std::string filename) {
  if (filename == "") filename = "GreatExpectations.txt";
  std::ifstream my_file;
  my_file.open(filename);
  if(!my_file.is_open()){
      std::cout << "File in trieTest() didn't open!\n";
  }
  std::cout << "Now Loading " << filename << "...\n";

  std::string temp_word;
  std::clock_t start = clock();
  std::string sequence = "";

  int count = 1;
  my_file >> sequence;
  // Add 5 words in queue together and add sequence to StringSequenceTrie
  while (!my_file.eof() && count < 10){
      my_file >> temp_word;
      if(temp_word != "----------------------------------------"){
          sequence += " " + temp_word;
          count++;
      }
  }

  addSequence(sequence);

  while (!my_file.eof()) {
      my_file >> temp_word;
      if(temp_word != "----------------------------------------"){
          int space = sequence.find(" ");
          sequence.erase(0, space + 1);
          sequence.append(" " + temp_word);
          addSequence(sequence);
      }
  }
  std::clock_t duration = clock() - start;
  std::cout << "Time taken: " << duration / (double)CLOCKS_PER_SEC << std::endl;
  my_file.close();
}
