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

#include "../include/stringtrie.h"

#include <vector>
#include <queue>
#include <map>
#include <utility>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <iomanip>

StringTrieNode::StringTrieNode(const char &input_char)
    : data(input_char), is_a_word(false), parent(nullptr), m_paths() {}

StringTrieNode::~StringTrieNode() {}

void StringTrie::removeSubTrie(StringTrieNode* current) {
  for (std::pair<const char, StringTrieNode*> &pair : current->m_paths)
    removeSubTrie(pair.second);

  if (current->is_a_word) {
    number_of_total_words -= m_record->getNumberOccurences(current);
    number_of_unique_words--;
    m_record->removeWord(current);
  }

  if (current != head) current->parent->m_paths.erase(current->data);

  delete current;
}

StringTrie::StringTrie() {
  head = new StringTrieNode('\0');
  m_record = new StringRecord();
}

StringTrie::~StringTrie() {
  removeSubTrie(head);
}

void StringTrie::resetTrie() {
  removeSubTrie(head);
  delete m_record;
  head = new StringTrieNode('\0');
  m_record = new StringRecord();
  number_of_total_words = 0;
  number_of_unique_words = 0;
}

void StringTrie::addWord(const std::string &word) {
  if (word == "" || word == " ") return;

  StringTrieNode* current_node = head;
  const char* c_string_word = word.c_str();
  for (int i = 0, length = word.length(); i < length; i++) {
    char key_char = tolower(c_string_word[i]);
    if (!current_node->hasSuffixNode(key_char)) {
      current_node->m_paths[key_char] = new StringTrieNode(key_char);
      current_node->m_paths[key_char]->parent = current_node;
    }
    current_node = current_node->m_paths[key_char];
  }

  if (!current_node->is_a_word) number_of_unique_words +=1;
  current_node->is_a_word = true;
  number_of_total_words += 1;

  m_record->addWord(current_node);
}

bool StringTrie::contains(const std::string &word) {
  if (word.length() == 0 || this->getNumberTotalWords() == 0) return false;
  StringTrieNode* current_node = head;
  const char* c_string_word = word.c_str();
  for (int i = 0, length = word.length(); i < length; i++) {
    char key_char = tolower(c_string_word[i]);
    current_node = current_node->getSuffixNode(key_char);
    if (current_node == nullptr) return false;
  }

  if (current_node->is_a_word)
    return true;
  else
    return false;
}

//
void StringTrie::remove(const std::string &word) {
  StringTrieNode* current_node = head;
  const char* c_string_word = word.c_str();

  // find node containing final character of string
  for (int i = 0, length = word.length(); i < length; i++) {
    char key_char = tolower(c_string_word[i]);
    current_node = current_node->getSuffixNode(key_char);
    if (current_node == nullptr) {
      printf("%s is not a word in the trie\n", c_string_word);
      return;
    }
  }

  current_node->is_a_word = false;

  // delete current node and any parent nodes that have no other child nodes
  while (current_node != head) {
    StringTrieNode* parent = current_node->parent;
    if (current_node->m_paths.size() != 0) return;
    parent->m_paths.erase(current_node->data);
    delete current_node;
    current_node = parent;
  }
}

void StringTrie::removeAllWithPrefix(const std::string &prefix) {
  StringTrieNode* current_node = head;
  const char* c_string_prefix = prefix.c_str();

  for (int i = 0, length = prefix.length(); i < length; i++) {
    char key_char = tolower(c_string_prefix[i]);
    current_node = current_node->getSuffixNode(key_char);
    if (current_node == nullptr) {
      std::cerr << "ERROR: No words with prefix: " << prefix << std::endl;
      return;
    }
  }
  removeSubTrie(current_node);
}

void StringTrie::printAll() const {
  printAllHelper(head, "");
}

// prints all words with a given prefix
// input: prefix of words to print
void StringTrie::printAllWithPrefix(const std::string &prefix) const {
  StringTrieNode* current_node = head;
  const char* c_string_prefix = prefix.c_str();
  char key_char = '\0';
  for (int i = 0, length = prefix.length(); i < length; i++) {
    key_char = tolower(c_string_prefix[i]);
    current_node = current_node->getSuffixNode(key_char);
    if (current_node == nullptr) {
      std::cerr << "ERROR: No words with prefix: " << prefix << std::endl;
      return;
    }
  }
  printAllHelper(current_node, prefix);
}

void StringTrie::printAllByOccurences() const {
  printOccurencesInRange();
}

void StringTrie::printOccurencesInRange(int upper_limit, int lower_limit) const {
  std::vector<std::pair<std::string, int>> pairs = m_record->getOrderedWords(this, upper_limit, lower_limit);
  std::cout << "Rank  | Frequency | String" << std::endl
            << "------|-----------|-------" << std::endl;
  int i = 1;
  for (std::pair<std::string, int> &pair : pairs)
    std::cout << std::setw(5) << i++ << std::setw(2)
          << "|" << std::setw(10) << pair.second << std::setw(2)
          << "|" << pair.first << std::endl;
}

void StringTrie::printTopOccurences(int limit) {
  std::vector<std::pair<std::string, int>> pairs = m_record->getOrderedWords(this);
  std::cout << "Rank  | Frequency | String" << std::endl
            << "------|-----------|-------" << std::endl;
  for (int i = 0; i < limit && i < (int)pairs.size(); i++)
    std::cout << std::setw(5) << i+1 << std::setw(2) << "|" << std::setw(10)
              << pairs[i].second << std::setw(2) << "|" << pairs[i].first << std::endl;
}

void StringTrie::writeToFile(std::string filename) const {
  std::ofstream outfile(filename);
  if (!outfile.is_open()) {
    std::cerr << "ERROR: " << filename << " could not be opened.\n";
    return;
  }
  outfile << head->m_paths.size() << " " << this->number_of_unique_words << " "
          << this->number_of_total_words << " ";
  for (const std::pair<char, StringTrieNode*> &pair : head->m_paths)
    writeToFileHelper(outfile, pair.second);
}

void StringTrie::writeToFileHelper(std::ofstream &outfile,
    const StringTrieNode *current_node) const {
  outfile << current_node->data << " "
          << (current_node->is_a_word ? m_record->getNumberOccurences(current_node) : 0)
          << " " << current_node->m_paths.size() << " ";
  for (const std::pair<char, StringTrieNode*> &pair : current_node->m_paths)
    writeToFileHelper(outfile, pair.second);
}

void StringTrie::readFromFile(std::string filename) {
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    std::cerr << "ERROR: " << filename << " could not be opened.\n";
    return;
  }
  int current_size = 0;
  infile >> current_size >> this->number_of_unique_words
         >> this->number_of_total_words;
  for (int i = 0; i < current_size; i++)
    readFromFileHelper(infile, head);
}

void StringTrie::readFromFileHelper(std::ifstream &infile,
                                    StringTrieNode *current_node) const {
  char current_char, current_frequency, current_size;
  infile >> current_char >> current_frequency >> current_size;

  if (!current_node->hasSuffixNode(current_char)) {
    current_node->m_paths[current_char] = new StringTrieNode(current_char);
    current_node->m_paths[current_char]->parent = current_node;
  }
  if (current_frequency > 0) {
    current_node->m_paths[current_char]->is_a_word = true;
    m_record->addWord(current_node->m_paths[current_char], current_frequency);
  }

  for (int i = 0; i < current_size; i++)
    readFromFileHelper(infile, current_node->m_paths[current_char]);
}

// prints all words in subtree,
// word is built up one character at a time with each rescursive call
void StringTrie::printAllHelper(StringTrieNode* current, std::string word) const {
  if (current->is_a_word) printf("%s\n", word);
  for (const auto& t_pair : current->m_paths)
    printAllHelper(t_pair.second, word + t_pair.first);
}

int StringTrie::getNumberTotalWords() const {
  return number_of_total_words;
}

int StringTrie::getNumberUniqueWords() const {
  return number_of_unique_words;
}

int StringTrie::getNumberOccurences(const std::string &word) {
  StringTrieNode* word_node = getNode(word);
  if (word_node != nullptr)
    return m_record->getNumberOccurences(word_node);
  else
    return 0;
}

std::string StringTrie::getLongestWord() const {
  int length = 0;
  StringTrieNode* final_node = head;
  findLongestWord(head, 0, length, final_node);
  return buildStringFromFinalNode(final_node);
}

// performs breadth first search through the trie
// returns the length of the shortest word in trie
int StringTrie::getLengthOfShortestWord() const {
  std::queue<StringTrieNode*> possible_words;
  StringTrieNode* current_node;
  int min_length = 1;
  int current_level_paths_left = 1;
  int next_level_paths = 0;
  possible_words.push(head);
  while (possible_words.size() > 0) {
    current_node = possible_words.front();
    possible_words.pop();
    for (auto iter : current_node->m_paths) {
      if (iter.second->is_a_word) return min_length;
      next_level_paths++;
      possible_words.push(iter.second);
    }
    current_level_paths_left--;
    if (current_level_paths_left == 0) {
      min_length++;
      current_level_paths_left = next_level_paths;
      next_level_paths = 0;
    }
  }
  return 0;
}

// constructs string character by character
std::string StringTrie::buildStringFromFinalNode(const StringTrieNode* current_node) const {
  if (current_node->data != '\0')
    return buildStringFromFinalNode(current_node->parent) + current_node->data;
  else
    return "";
}

// returns a pointer to the StringTrieNode corresponding with the last character
// of the string word. If word is not in the trie, a nullptr is returned
StringTrieNode *StringTrie::getNode(const std::string &word) {
  StringTrieNode* current_node = head;
  const char* c_string_word = word.c_str();
  char key_char = '\0';
  for (int i = 0, length = word.length(); i < length; i++) {
    key_char = tolower(c_string_word[i]);
    current_node = current_node->getSuffixNode(key_char);
    if (current_node == nullptr) return nullptr;
  }

  if (current_node->is_a_word)
    return current_node;
  else
    return nullptr;
}

// returns longest word in trie
//
void StringTrie::findLongestWord(StringTrieNode* current_node, int current_length,
                 int &longest_length, StringTrieNode *&longest) const {

  for (const auto& next_node : current_node->m_paths)
    findLongestWord(next_node.second, current_length+1, longest_length, longest);

  if (current_node->is_a_word && current_length > longest_length) {
    longest_length = current_length;
    longest = current_node;
  }
}

int StringTrie::getLengthOfLongestWord() const {
  int max_length = 0;
  StringTrieNode* final_node = head;

  findLongestWord(head, 0, max_length, final_node);
  std::string word = buildStringFromFinalNode(final_node);
  std::cout << word << std::endl;
  return max_length;
}


void StringRecord::addWord(const StringTrieNode *current_node, int occurences) {
  if (m_record.find(current_node) == m_record.end())
    m_record[current_node] = occurences;
  else
    m_record[current_node] += occurences;
}

void StringRecord::removeWord(const StringTrieNode *current_node) {
  m_record.erase(current_node);
}

int StringRecord::getNumberOccurences(const StringTrieNode *current_node) const {
  // checks if node is in hash table and returns value if it exists, zero otherwise
  return m_record.find(current_node) != m_record.end() ? m_record.find(current_node)->second : 0;
}


std::vector<std::pair<std::string, int>> StringRecord::getOrderedWords(
    const StringTrie *trie,
    const int upper_limit,
    const int lower_limit) {
  std::vector<std::pair<std::string, int>> words;
  int word_occurences = 0;
  std::string word = "";

  // add pair to vector if seen more than 40 times
  for (const std::pair<const StringTrieNode*, int> &pair : m_record) {
    if (pair.second < lower_limit || pair.second > upper_limit) continue;
    word = trie->buildStringFromFinalNode(pair.first);
    word_occurences = pair.second;
    words.push_back(std::pair<std::string, int>(std::move(word), word_occurences));
  }

  // sort vector by the second value in pair
  std::sort(words.begin(), words.end(),
        [](const std::pair<std::string, int> &left,
           const std::pair<std::string, int> &right) {
    return left.second > right.second;
  });
  return words;
}

std::vector<int> StringRecord::getOrderedOccurences() const {
  std::vector<int> occurences;
  for (const auto &pair : m_record)
    occurences.push_back(pair.second);
  std::sort(occurences.begin(), occurences.end(), std::greater<int>());
  return occurences;
}
