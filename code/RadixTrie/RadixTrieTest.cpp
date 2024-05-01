// RadixTrieTest.cpp
#include "RadixTrie.h"
#include <iostream>

void testInsertAndSearch() {
    RadixTree<int> tree;
    tree.put("apple", 10);

    bool test1 = (tree.getValueForExactKey("apple") == 10);
    bool test2 = (tree.getValueForExactKey("app") == int());

    std::cout << "Test Insert and Search: "
              << (test1 && test2 ? "PASSED" : "FAILED") << std::endl;
}

void testCollectPairs() {
    RadixTree<int> tree;
    tree.put("apple", 10);
    tree.put("app", 5);
    tree.put("ape", 7);

    auto pairs = tree.collectPairs("ap");
    bool test = (pairs.size() == 3);

    std::cout << "Test Collect Pairs: "
              << (test ? "PASSED" : "FAILED") << std::endl;
}

void testGetKeysStartingWith() {
    RadixTree<int> tree;
    tree.put("apple", 10);
    tree.put("app", 5);

    auto keys = tree.getKeysStartingWith("app");
    bool test = (keys.size() == 2 && find(keys.begin(), keys.end(), "apple") != keys.end() && find(keys.begin(), keys.end(), "app") != keys.end());

    std::cout << "Test Get Keys Starting With: "
              << (test ? "PASSED" : "FAILED") << std::endl;
}

int main() {
    testInsertAndSearch();
    testCollectPairs();
    testGetKeysStartingWith();
    return 0;
}
