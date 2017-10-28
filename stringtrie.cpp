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

#include "stringtrie.h"

#include <vector>
#include <queue>
#include <map>
#include <utility>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>

StringTrieNode::StringTrieNode(char input_char)
  : data(input_char),
    m_paths(),
    parent(nullptr),
    is_a_word(false) {}

StringTrieNode::~StringTrieNode() {
}

void StringTrie::removeSubTrie(StringTrieNode* current) {
    for (const std::pair<char, StringTrieNode*>& iter : current->m_paths) {
        removeSubTrie(iter.second);
    }
    delete current;
}

StringTrie::StringTrie() {
    head = new StringTrieNode('\0');
    m_record = new StringRecord();
}

StringTrie::~StringTrie() {
    removeSubTrie(head);
}

void StringTrie::addWord(const std::string &word) {
    if (word == "" || word == " ") return;
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    StringTrieNode* current_node = head;
    const char* c_string_word = word.c_str();
    int size = word.length();
    for (int i = 0; i < size; i++) {
        char key_char = tolower(c_string_word[i]);
        std::map<char, StringTrieNode*>::const_iterator found(current_map->find(key_char));
        if (found == current_map->end()) {
            StringTrieNode* t_node_pointer = new StringTrieNode(key_char);
            t_node_pointer->parent = current_node;
            std::pair<char, StringTrieNode*> t_pair(key_char, t_node_pointer);
            current_map->insert(t_pair);
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }

    if (!current_node->is_a_word) number_of_unique_words +=1;
    current_node->is_a_word = true;
    number_of_total_words += 1;

    m_record->addWord(current_node);
}

bool StringTrie::contains(const std::string &word) {
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    if (current_map->size() == 0) return false;
    StringTrieNode* current_node = head;
    const char* c_string_word = word.c_str();
    char key_char = -1;
    int size = word.length();
    for (int i = 0; i < size; i++) {
        key_char = tolower(c_string_word[i]);
        if (current_map->find(key_char) == current_map->end()) {
            return false;
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }
    if (current_node->is_a_word) {
        return true;
    } else {
        return false;
    }
}

//
void StringTrie::remove(const std::string &word) {
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    std::map<char, StringTrieNode*>* parent_map = &head->m_paths;
    std::vector<StringTrieNode*> potential_deletions;
    StringTrieNode* current_node = head;
    int size = word.size();
    const char* c_string_word = word.c_str();
    char key_char = -1;
    for (int i = 0; i < size; i++) {
        potential_deletions.push_back(current_node);
        key_char = tolower(c_string_word[i]);
        if (current_map->find(key_char) == current_map->end()) {
            printf("%s is not a word in the trie\n", c_string_word);
            return;
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }
    potential_deletions.push_back(current_node);

    while (potential_deletions.size() >= 1) {
        current_node = potential_deletions.back();
        potential_deletions.pop_back();
        parent_map = &potential_deletions.back()->m_paths;

        if (current_node->m_paths.size() == 0) {
            parent_map->at(key_char) = nullptr;
            delete current_node;
        } else {
            current_node->is_a_word = false;
            return;
        }
    }
}

void StringTrie::removeAllWithPrefix(const std::string &prefix) {
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    StringTrieNode* current_node = head;
    const char* c_string_prefix = prefix.c_str();
    char key_char = -1;
    int size = prefix.length();
    for (int i = 0; i < size; i++) {
        key_char = tolower(c_string_prefix[i]);
        if (current_map->find(key_char) == current_map->end()) {
            std::cout << "\nNo words with prefix: " << prefix << std::endl;
            return;
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }
    removeSubTrie(current_node);
}

void StringTrie::printAll() {
    printAllHelper(head, "");
}

// prints all words with a given prefix
// input: prefix of words to print
void StringTrie::printAllWithPrefix(const std::string &prefix) {
    std::map<char, StringTrieNode*>* current_map = &head->m_paths;
    StringTrieNode* current_node = head;
    const char* c_string_prefix = prefix.c_str();
    char key_char = -1;
    int size = prefix.length();
    for (int i = 0; i < size; i++) {
        key_char = tolower(c_string_prefix[i]);
        if (current_map->find(key_char) == current_map->end()) {
            std::cout << "\nNo words with prefix: " << prefix << std::endl;
            return;
        }
        current_node = current_map->at(key_char);
        current_map = &current_node->m_paths;
    }
    printAllHelper(current_node, prefix);
}

// prints all words in subtree,
// word is built up one character at a time with each rescursive call
void StringTrie::printAllHelper(StringTrieNode* current, std::string word) const {
    if (current->is_a_word) {
        std::cout << word << std::endl;
    }
    for (const auto& t_pair : current->m_paths) {
        printAllHelper(t_pair.second, word + t_pair.first);
    }
}

int StringTrie::getNumberTotalWords() const {
    return number_of_total_words;
}

int StringTrie::getNumberUniqueWords() const {
    return number_of_unique_words;
}

std::string StringTrie::getLongestWord() const {
    int length = 0;
    StringTrieNode* final_node = head;
    findLongestWord(head, 0, length, final_node);
    return buildStringFromFinalNode(final_node);
}

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
            if (iter.second->is_a_word) {
                return min_length;
            }
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
  if (current_node->data != '\0') {
      return buildStringFromFinalNode(current_node->parent) + current_node->data;
  } else {
      return "";
  }
}


StringTrieNode *StringTrie::getNode(const std::string &word) {
  std::map<char, StringTrieNode*>* current_map = &head->m_paths;
  StringTrieNode* current_node = head;
  const char* c_string_word = word.c_str();
  char key_char = -1;
  int size = word.length();
  for (int i = 0; i < size; i++) {
      key_char = tolower(c_string_word[i]);
      if (current_map->find(key_char) == current_map->end()) {
          return nullptr;
      }
      current_node = current_map->at(key_char);
      current_map = &current_node->m_paths;
  }
  if (current_node->is_a_word) {
      return current_node;
  } else {
      return nullptr;
  }
}

// returns longest word in trie
//
void StringTrie::findLongestWord(StringTrieNode* current_node, int current_length,
                                 int &longest_length, StringTrieNode *&longest) const {
    for (const auto& next_node : current_node->m_paths) {
        findLongestWord(next_node.second, current_length+1, longest_length, longest);
    }
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


void StringRecord::addWord(StringTrieNode *current_node) {
    auto it = m_record.find(current_node);
    if (it == m_record.end()) {
        m_record[current_node] = 1;
    } else {
        it->second += 1;
    }
}


std::vector<std::pair<std::string, int>> StringRecord::getOrderedWords(const StringTrie *trie,
                                                                       const int lower_limit,
                                                                       const int upper_limit) {
    std::vector<std::pair<std::string, int>> words;
    int word_occurences = 0;
    std::string word = "";

    // add pair to vector if seen more than 40 times
    for (const std::pair<StringTrieNode*, int> &pair : m_record) {
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

std::vector<int> StringRecord::getOrderedOccurences() {
    std::vector<int> occurences;
    for (const auto &pair : m_record) {
        occurences.push_back(pair.second);
    }
    std::sort(occurences.begin(), occurences.end(), std::greater<int>());
    return occurences;
}

StringSequenceTrie::StringSequenceTrie() : m_total_words(0),
                                           m_trie(new StringTrie()),
                                           head(new StringSequenceTrieNode(m_trie->head, nullptr)) {}

void StringSequenceTrie::addSequence(const std::string &sequence) {
    addSequenceHelper(sequence, 0, true);
    // std::cout << "Adding to Sequence Trie: " << sequence << std::endl;
}

void StringSequenceTrie::addSequenceHelper(const std::string &sequence,
                                             short starting_pos,
                                             bool add_word_to_trie) {

    // verify we have a sequence to look at
    std::size_t size = sequence.size();

    std::size_t space_index = -1;

    std::string str = "";
    StringTrieNode* current_trie_node = nullptr;
    StringSequenceTrieNode* current_sequence_node = head;
    StringSequenceTrieNode* parent = head;
    do {
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
     } while (space_index < size);
}

const StringSequenceTrieNode *StringSequenceTrie::getNode(std::string &sequence)
{
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


void StringSequenceTrie::loadTextFile(std::string file_name) {
  if (file_name == "") file_name = "GreatExpectations.txt";
  std::ifstream my_file;
  my_file.open(file_name);
  if(!my_file.is_open()){
      std::cout << "File in trieTest() didn't open!\n";
  }
  std::cout << "Now Loading " << file_name << "...\n";

  char* c;
  std::string temp_word;
  std::clock_t start = clock();
  std::string sequence = "";

  int count = 1;
  my_file >> sequence;
  // Add 5 words in queue together and add sequence to StringSequenceTrie
  while (!my_file.eof() && count < 5){
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

