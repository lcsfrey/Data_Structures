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

StringSequenceTrieNode::StringSequenceTrieNode(StringTrieNode *string_trie_node,
                                               StringSequenceTrieNode *parent)  : m_trie_word_node(string_trie_node),
                                                                                  m_parent(parent), m_next_word(),
                                                                                  m_times_seen(1) {}


StringSequenceTrie::StringSequenceTrie() : m_trie(new StringTrie()), head(new StringSequenceTrieNode(m_trie->head, nullptr)),
                                           m_total_words(0) {}

void StringSequenceTrie::addSequence(const std::string &sequence) {
    addSequenceHelper(sequence, head, 0);
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

std::vector<StringSequenceTrieNode*> StringSequenceTrie::getOrderedWords(const std::string sequence,
                                                                         int sequence_length_upper_limit,
                                                                         int sequence_length_lower_limit,
                                                                         int branching_factor) {
    std::vector<StringSequenceTrieNode*> sequences;
    StringSequenceTrieNode* current_node = nullptr;

    if (sequence == "") current_node = head;
    else current_node = getNode(sequence);

    if (current_node == nullptr) return sequences;

    getOrderedWordsHelper(current_node, sequences,
                          sequence_length_upper_limit, sequence_length_lower_limit,
                          1, branching_factor);

    std::sort(sequences.begin(), sequences.end(),
              [](const StringSequenceTrieNode* left,
                 const StringSequenceTrieNode* right) {
        return left->m_times_seen > right->m_times_seen;
    });

    return sequences;
}

void StringSequenceTrie::getOrderedWordsHelper(const StringSequenceTrieNode *current_node,
                                               std::vector<StringSequenceTrieNode*> &sequences,
                                               const int &sequence_length_upper_limit,
                                               const int &sequence_length_lower_limit,
                                               int current_sequence_length,
                                               int branching_factor) {

    if (current_sequence_length > sequence_length_upper_limit) return;

    std::vector<StringSequenceTrieNode*> words;

    for (const std::pair<StringTrieNode*, StringSequenceTrieNode*> &pair : current_node->m_next_word) {
        words.push_back(pair.second);
    }
    if (words.size() == 0) return;

    // sort words by the number of times they have been seen
    std::sort(words.begin(), words.end(),
              [](const StringSequenceTrieNode* left,
                 const StringSequenceTrieNode* right) {
        return left->m_times_seen > right->m_times_seen;
    });

    int size = std::min((int)words.size(), branching_factor);

    for (int i = 0; i < size; i++) {
        getOrderedWordsHelper(words[i], sequences,
                              sequence_length_upper_limit, sequence_length_lower_limit,
                              current_sequence_length + 1, branching_factor);
    }
    if (current_sequence_length >= sequence_length_lower_limit) {
        std::move(words.begin(), words.begin() + size, std::back_inserter(sequences));
    }
}

void StringSequenceTrie::printOrderedWords(const std::string sequence,
                                           int sequence_length_upper_limit,
                                           int sequence_length_lower_limit,
                                           int branching_factor,
                                           int frequency_upper_limit,
                                           int frequency_lower_limit) {
    std::vector<StringSequenceTrieNode*> sequences = getOrderedWords(sequence,
                                                                     sequence_length_upper_limit,
                                                                     sequence_length_lower_limit,
                                                                     branching_factor);
    if (sequence != "") {
        std::cout << "Showing sequences starting with \"" << sequence << "\"\n";
    }
    std::cout << "Frequency | Sequence"  << std::endl
              << "----------|-----------" << std::endl;

    for (const StringSequenceTrieNode* node : sequences) {
        if (node->m_times_seen >= frequency_lower_limit && node->m_times_seen <= frequency_upper_limit) {
            std::cout << std::setw(9) << node->m_times_seen << " | " << buildSequenceFromFinalNode(node) << std::endl;
        }
    }
}

std::string StringSequenceTrie::buildSequenceFromFinalNode(const StringSequenceTrieNode *current) const {
    if (current != head) {
        return buildSequenceFromFinalNode(current->m_parent) + m_trie->buildStringFromFinalNode(current->m_trie_word_node) + " ";
    } else {
        return "";
    }
}

void StringSequenceTrie::addSequenceHelper(const std::string &sequence, StringSequenceTrieNode* current_node,
                                           int starting_pos) {

    // verify we have a sequence to look at
    std::size_t size = sequence.size();

    std::size_t space_index = -1;

    std::string str = "";
    StringTrieNode* current_trie_node = nullptr;
    StringSequenceTrieNode* current_sequence_node = current_node;
    StringSequenceTrieNode* parent = current_node;
    space_index = sequence.find(" ", starting_pos);

    // get the next word in the sequence
    if (space_index < size) {
        str = sequence.substr(starting_pos, space_index - starting_pos);
    } else {
        str = sequence.substr(starting_pos, size - starting_pos);
    }
    m_trie->addWord(str);
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

    // add sequence of words starting after current word
    addSequenceHelper(sequence, current_sequence_node, (int)space_index + 1);
    starting_pos = space_index + 1;
}

StringSequenceTrieNode* StringSequenceTrie::getNode(const std::string &sequence) const {
    // verify we have a sequence to look at
    std::size_t size = sequence.size();

    std::size_t space_index = -1;

    std::string str = "";
    StringTrieNode* current_trie_node = nullptr;
    int current_pos = 0;
    StringSequenceTrieNode* current_record_node = head;

    do {
        // find the next space in the sentence
        space_index = sequence.find(" ", current_pos);
        // check and verify there is a space in the string, else return
        if (space_index < size) {
            str = sequence.substr(current_pos, space_index - current_pos);
        } else {
            str = sequence.substr(current_pos, size - current_pos);
        }
        current_pos = space_index + 1;

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


void StringSequenceTrie::loadTextFile(std::string file_name, int window_size) {
    if (file_name == "") file_name = "GreatExpectations.txt";
    std::ifstream my_file;
    my_file.open(file_name);
    if(!my_file.is_open()){
        std::cout << "File in trieTest() didn't open!\n";
    }
    std::cout << "Now Loading " << file_name << "...\n";

    std::string temp_word;
    std::clock_t start = clock();
    std::string sequence = "";

    int count = 1;
    my_file >> sequence;
    // Add 5 words in queue together and add sequence to StringSequenceTrie
    while (!my_file.eof() && count < window_size){
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
    std::cout << "Time taken: " << duration / (double)CLOCKS_PER_SEC << " seconds" << std::endl;
    my_file.close();
}
