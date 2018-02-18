#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../include/stringtrie.h"

using namespace testing;

TEST(teststringtrie, testContains) {
    StringTrie trie;
    EXPECT_EQ(false, trie.contains("apple"));
    trie.addWord("apple");
    EXPECT_EQ(true, trie.contains("apple"));
}

TEST(teststringtrie, testAddMultipleWords) {
    StringTrie trie;

    // validate that trie is empty
    EXPECT_EQ(0, trie.getNumberUniqueWords());
    EXPECT_EQ(0, trie.getNumberTotalWords());

    // add two to total, two to unique
    trie.addWord("apple");
    trie.addWord("banana");
    EXPECT_EQ(2, trie.getNumberUniqueWords());
    EXPECT_EQ(2, trie.getNumberTotalWords());

    // add one to total, zero to unique
    trie.addWord("apple");
    EXPECT_EQ(2, trie.getNumberUniqueWords());
    EXPECT_EQ(3, trie.getNumberTotalWords());

    // add two to total, two to unique
    trie.addWord("orange");
    trie.addWord("grape");
    EXPECT_EQ(4, trie.getNumberUniqueWords());
    EXPECT_EQ(5, trie.getNumberTotalWords());

    // add three to total, zero to unique
    trie.addWord("orange");
    trie.addWord("grape");
    trie.addWord("banana");
    EXPECT_EQ(4, trie.getNumberUniqueWords());
    EXPECT_EQ(8, trie.getNumberTotalWords());
}

TEST(teststringtrie, testRemoval) {
    StringTrie trie;
    EXPECT_FALSE(trie.contains("apple"));
    trie.addWord("apple");
    EXPECT_TRUE(trie.contains("apple"));
    trie.remove("apple");
    EXPECT_FALSE(trie.contains("apple"));
}

TEST(teststringtrie, testLongestWord) {
    StringTrie trie;
    EXPECT_EQ("", trie.getLongestWord());

    trie.addWord("apple");
    trie.addWord("banana");
    EXPECT_EQ("banana", trie.getLongestWord());

    trie.addWord("pineapple");
    EXPECT_EQ("pineapple", trie.getLongestWord());
}

TEST(teststringtrie, testClearTrie) {
    StringTrie trie;
    trie.addWord("apple");
    trie.addWord("apple");
    trie.addWord("apples");
    trie.addWord("banana");
    trie.addWord("bananas");
    trie.addWord("orange");
    trie.addWord("grape");
    EXPECT_EQ(7, trie.getNumberTotalWords());
    EXPECT_EQ(6, trie.getNumberUniqueWords());
    trie.resetTrie();
    EXPECT_EQ(0, trie.getNumberTotalWords());
    EXPECT_EQ(0, trie.getNumberUniqueWords());
}

TEST(teststringtrie, testRemoveAllWithPrefix) {
    StringTrie trie;
    trie.addWord("apple");
    trie.addWord("apple");
    trie.addWord("apples");
    trie.addWord("applesss");
    trie.addWord("banana");
    trie.addWord("pineapple");
    trie.removeAllWithPrefix("apple");
    EXPECT_EQ(2, trie.getNumberUniqueWords());
}

TEST(teststringtrie, testNumberOccurences) {
    StringTrie trie;
    EXPECT_EQ(0, trie.getNumberOccurences("apple"));
    trie.addWord("apple");
    trie.addWord("apples");
    trie.addWord("applesss");
    trie.addWord("banana");
    trie.addWord("pineapple");
    trie.addWord("apple");
    trie.addWord("pear");
    trie.addWord("apple");
    trie.addWord("orange");
    EXPECT_EQ(3, trie.getNumberOccurences("apple"));
    EXPECT_EQ(1, trie.getNumberOccurences("apples"));
    EXPECT_EQ(1, trie.getNumberOccurences("pear"));
}

