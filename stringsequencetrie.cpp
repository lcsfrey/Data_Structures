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

#include <ctime>
#include <fstream>
#include <string>
#include <utility>
#include <map>
#include <algorithm>
#include <iomanip>

#include "stringsequencetrie.h"
#include "stringtrie.h"

StringSequenceTrieNode::StringSequenceTrieNode(
    StringTrieNode *string_trie_node, StringSequenceTrieNode *parent)
    : m_trie_word_node(string_trie_node), m_parent(parent), m_next_word(),
      m_times_seen(1) {}


StringSequenceTrie::StringSequenceTrie() : m_trie(new StringTrie()),
    m_seq_head(new StringSequenceTrieNode(m_trie->m_trie_head, nullptr)),
    m_seq_backward_head(new StringSequenceTrieNode(m_trie->m_trie_head, nullptr)),
    m_total_words(0) {}

void StringSequenceTrie::addSequence(const std::string &sequence) {
  addSequenceHelper(sequence, m_seq_head, 0);
  addSequenceBackwardHelper(sequence, m_seq_backward_head, sequence.size());
}

void StringSequenceTrie::addSequence(const std::vector<std::string> &sequence,
                                     int window_size) {
  for (int i = 0; i < window_size; i++)
    m_trie->addWord(sequence[i]);

  for (int i = 0, size = sequence.size(); i < size - window_size; i++) {
    // m_trie->addWord(sequence[i]);
    StringSequenceTrieNode* current_sequence_node = m_seq_head;
    StringTrieNode* current_trie_node = nullptr;
    for (int k = i; k < i + window_size; k++) {
      current_trie_node = m_trie->getNode(sequence[k]);
      if (!current_sequence_node->containsNextWord(current_trie_node)) {
        current_sequence_node->addChild(current_trie_node);
      }
      current_sequence_node = current_sequence_node->m_next_word[current_trie_node];
    }
  }
}

std::string StringSequenceTrie::getNextWord(std::string &sequence) {
  StringSequenceTrieNode* current = getNode(sequence);
  StringSequenceTrieNode* next_word = nullptr;
  int max = 0;

  for (const auto &pair : current->m_next_word) {
      if (max < pair.second->getTimesSeen()) {
          max = max < pair.second->getTimesSeen();
          next_word = pair.second;
      }
  }
  return m_trie->buildStringFromFinalNode(next_word->getWordNode());
}

std::vector<StringSequenceTrieNode*> StringSequenceTrie::getOrderedWords(
    const SequenceCriteria & criteria) const {

  StringSequenceTrieNode* current_node = nullptr;
  if (criteria.m_starting_sequence == "")
    current_node = m_seq_head;
  else
    current_node = getNode(criteria.m_starting_sequence);

  if (current_node == nullptr) return std::vector<StringSequenceTrieNode*>();

  std::vector<StringSequenceTrieNode*> sequences;
  getOrderedWordsHelper(current_node, &sequences, criteria, 1);

  std::sort(sequences.begin(), sequences.end(),
            [](const StringSequenceTrieNode* left,
               const StringSequenceTrieNode* right) {
    return left->m_times_seen > right->m_times_seen;
  });

  return sequences;
}

void StringSequenceTrie::getOrderedWordsHelper(
    const StringSequenceTrieNode *current_node,
    std::vector<StringSequenceTrieNode*> *sequences,
    const SequenceCriteria & criteria,
    int current_sequence_length) const {

  if (current_sequence_length > criteria.m_length_max_count) return;

  std::vector<StringSequenceTrieNode*> words;

  for (const auto &next_word : current_node->m_next_word)
    words.push_back(next_word.second);

  if (words.size() == 0) return;

  // sort words by the number of times they have been seen
  std::sort(words.begin(), words.end(),
            [](const StringSequenceTrieNode* left,
               const StringSequenceTrieNode* right) {
    return left->m_times_seen > right->m_times_seen;
  });

  int size = std::min((int)words.size(), criteria.m_branching_factor);
  for (int i = 0; i < size; i++) {
    getOrderedWordsHelper(words[i], sequences, criteria,
                          current_sequence_length+1);
  }

  if (current_sequence_length >= criteria.m_length_min_count)
    std::move(words.begin(), words.begin() + size,
              std::back_inserter(*sequences));
}

void StringSequenceTrie::printOrderedWords(const SequenceCriteria &criteria) const {

  std::vector<StringSequenceTrieNode*> sequences = getOrderedWords(criteria);

  if (criteria.m_starting_sequence != "")
    std::cout << "Showing sequences starting with \""
              << criteria.m_starting_sequence << "\"\n";

  std::cout << "Rank  | Frequency | String" << std::endl
            << "------|-----------|-------" << std::endl;

  for (int i = 0, length = sequences.size(); i < length; i++)
    if (sequences[i]->m_times_seen >= criteria.m_frequency_min &&
        sequences[i]->m_times_seen <= criteria.m_frequency_max)
      std::cout << std::setw(5) << i+1 << std::setw(2) << "|" << std::setw(10)
                << sequences[i]->m_times_seen << std::setw(2) << "|"
                << buildSequenceFromFinalNode(sequences[i]) << std::endl;
}

void StringSequenceTrie::printMostFrequentSequences(int limit) const {
  std::vector<StringSequenceTrieNode*> sequences = getOrderedWords(SequenceCriteria());
  std::cout << "Rank  | Frequency | String" << std::endl
            << "------|-----------|-------" << std::endl;

  for (int i = 0, size = sequences.size(); i < limit && i < size; i++)
    std::cout << std::setw(5) << i << std::setw(2) << "|" << std::setw(10)
              << sequences[i]->m_times_seen << std::setw(2) << "|"
              << buildSequenceFromFinalNode(sequences[i]) << std::endl;
}

std::string StringSequenceTrie::buildSequenceFromFinalNode(
    const StringSequenceTrieNode *current) const {
  if (current != m_seq_head) {
    return buildSequenceFromFinalNode(current->m_parent)
      + m_trie->buildStringFromFinalNode(current->m_trie_word_node) + " ";
  } else {
    return "";
  }
}

void StringSequenceTrie::writeToFile(std::string filename) const {
  std::ofstream outfile(filename);
  if (!outfile.is_open()) {
    std::cerr << "ERROR: Couldn't open " + filename << std::endl;
    return;
  }

  outfile << m_seq_head->m_next_word.size() << std::endl;
  for(const auto &starting_word : m_seq_head->m_next_word)
    writeToFileHelper(outfile, starting_word.second);

  outfile.close();
}

void StringSequenceTrie::writeToFileHelper(std::ofstream &outfile,
    const StringSequenceTrieNode *current_node) const {

  int current_size = current_node->m_next_word.size();
  outfile << m_trie->buildStringFromFinalNode(current_node->m_trie_word_node)
          << " " << current_node->m_times_seen << " " << current_size << " ";

  if (current_size != 0) {
    for(const auto starting_word : current_node->m_next_word)
      writeToFileHelper(outfile, starting_word.second);
  } else {
    outfile << std::endl;
  }
}

//void StringSequenceTrie::sequenceAdder() {
//  while(true) {
//    if(word_queue.size() > m_window_size) {
//      auto it = word_queue.begin();
//      std::advance(it, m_window_size);
//      std::string str;
//      str.append(word_queue.begin(), it);
//    }
//  }
//}

void StringSequenceTrie::readFromFile(std::string filename) {
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    std::cout << "ERROR: Couldn't open " + filename << std::endl;
    return;
  }
  int branches = 0;
  infile >> branches;
  for (int i = 0; i < branches; i++)
    readFromFileHelper(infile, m_seq_head);

  infile.close();
}

void StringSequenceTrie::readFromFileHelper(std::ifstream &infile,
                      StringSequenceTrieNode *current_seq_node) {

  // read in one node from file
  int current_frequency = 0, current_branches = 0;
  std::string current_str;
  infile >> current_str >> current_frequency >> current_branches;

  // add word to trie if not already there
  if (!m_trie->contains(current_str)) m_trie->addWord(current_str);

  // get pointer to word
  StringTrieNode* current_trie_node = m_trie->getNode(current_str);

  if (!current_seq_node->containsNextWord(current_trie_node))
    current_seq_node->addChild(current_trie_node);

  current_seq_node->addTimesSeen(current_frequency - 1);

  //recursively call self on all child nodes
  for (int i = 0; i < current_branches; i++)
    readFromFileHelper(infile, current_seq_node->m_next_word[current_trie_node]);
}


void StringSequenceTrie::addSequenceHelper(const std::string &sequence,
    StringSequenceTrieNode* current_seq_node, std::size_t starting_pos) {

  // get the first word in the sequence
  std::size_t size = sequence.size();
  std::size_t space_index = sequence.find(" ", starting_pos);
  std::string str = "";
  if (space_index < size) {
    str = sequence.substr(starting_pos, space_index - starting_pos);
  } else {
    str = sequence.substr(starting_pos, size - starting_pos);
  }

  m_trie->addWord(str);
  StringTrieNode* current_trie_node = m_trie->getNode(str);

  if (!current_seq_node->containsNextWord(current_trie_node)) {
    current_seq_node->addChild(current_trie_node);
  }

  if (space_index >= size) return;

  // add sequence of words starting after current word
  addSequenceHelper(sequence, current_seq_node->m_next_word[current_trie_node],
                    space_index + 1);
}

void StringSequenceTrie::addSequenceBackwardHelper(
    const std::string &sequence,
    StringSequenceTrieNode *current_seq_node,
    std::size_t starting_pos) {
  // get the first word in the sequence
  std::size_t space_index = sequence.rfind(" ", starting_pos);
  std::string str = "";

  if (space_index < starting_pos) {
    str = sequence.substr(space_index + 1, starting_pos - space_index);
  } else {
    str = sequence.substr(0, starting_pos);
  }

  //m_trie->addWord(str);
  StringTrieNode* current_trie_node = m_trie->getNode(str);

  if (!current_seq_node->containsNextWord(current_trie_node)) {
    current_seq_node->addChild(current_trie_node);
  }

  if (space_index >= starting_pos) return;

  // add sequence of words starting after current word
  addSequenceBackwardHelper(sequence, current_seq_node->m_next_word[current_trie_node],
                            space_index - 1);
}

StringSequenceTrieNode* StringSequenceTrie::getNode(const std::string &sequence) const {
  StringSequenceTrieNode* current_record_node = m_seq_head;
  std::size_t space_index = 0, size = 0;
  int current_pos = 0;

  do {
    // find the next space in the sentence
    std::size_t space_index = sequence.find(" ", current_pos);
    std::size_t size = sequence.size();

    // check and verify there is a space in the string, else return
    std::string str;
    if (space_index < size)
      str = sequence.substr(current_pos, space_index - current_pos);
    else
      str = sequence.substr(current_pos, size - current_pos);

    current_pos = space_index + 1;

    // check if the word is in m_trie
    // if it is not, then it wouldn't be in the sequence record
    StringTrieNode* current_trie_node = m_trie->getNode(str);
    if (current_trie_node == nullptr) return nullptr;

    if (!current_record_node->containsNextWord(current_trie_node))
      return nullptr;
    else
      current_record_node = current_record_node->m_next_word[current_trie_node];
  } while (space_index < size);

  return current_record_node;
}

bool isPunctNotPeriod(const char &c) {
  return (std::ispunct(c) && c != '.') ? true : false;
}

void cleanString(std::string &str) {
  std::string output;
  std::remove_copy_if(str.begin(), str.end(), std::back_inserter(output),
                      std::ptr_fun(isPunctNotPeriod));
  str.assign(std::move(output));
}

void StringSequenceTrie::loadTextFile(std::string file_name, int window_size) {
  if (file_name == "") file_name = "books/Mark_Twain_LifeOnTheMississippi.txt";
  else file_name = "books/" + file_name;
  std::ifstream my_file;
  my_file.open(file_name);

  if(!my_file.is_open())
    std::cerr << "ERROR: " << file_name << " didn't open!\n";

  std::cout << "Now Loading " << file_name << "...\n";

  std::string temp_word;
  std::clock_t start = clock();
  std::string sequence = "";

  int count = 1;
  my_file >> sequence;
  // Add 5 words in queue together and add sequence to StringSequenceTrie
  while (!my_file.eof() && count < window_size){
    my_file >> temp_word;
    cleanString(temp_word);
    if (temp_word != "" && temp_word != " ") {
      sequence += " " + temp_word;
      count++;
    }
  }

  addSequence(sequence);

  while (!my_file.eof()) {
    my_file >> temp_word;
    cleanString(temp_word);
    if(temp_word != ""){
      size_t space = sequence.find(" ");
      sequence.erase(0, space + 1);
      sequence += " " + temp_word;
      addSequence(sequence);
    }
  }
  double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
  std::cout << "Time taken: " << duration  << " seconds" << std::endl;
  my_file.close();
}

void StringSequenceTrie::SequenceCriteria::setStartingSequence(std::string starting_sequence) {
  m_starting_sequence = starting_sequence;
  for(int i = 0, size = starting_sequence.size() - 1; i < size; i++) {
    if (starting_sequence[i+1] == ' ') {
      if (starting_sequence[i] != ' ') {
        this->m_length_max_count--;
        this->m_length_min_count--;
      }
    }
  }
}
