#include "RedixTrie.h"
#include <iostream>

class RedixTrieTest {
public:
    RedixTrie trie;

    RedixTrieTest() : trie() {}

    void TestInsertAndSearch() {
        std::cout << "Running TestInsertAndSearch..." << std::endl;

        // Test insertion of strings
        trie.insert("hello");
        trie.insert("world");
        trie.insert("helium");

        // Check and print results
        CheckResult("hello", trie.search("hello"));
        CheckResult("world", trie.search("world"));
        CheckResult("help", trie.search("help") == false);  // "help" should not be found
        CheckResult("helium", trie.search("helium"));
    }

    void TestSearchNonExistentString() {
        std::cout << "Running TestSearchNonExistentString..." << std::endl;

        // Test searching for a string that has not been inserted
        CheckResult("nonexistent", trie.search("nonexistent") == false);
    }

    void TestEmptyTrie() {
        std::cout << "Running TestEmptyTrie..." << std::endl;

        // Test searching in an empty trie
        RedixTrie emptyTrie;  // Create a new empty trie
        CheckResult("", emptyTrie.search("") == false);
        CheckResult("anything", emptyTrie.search("anything") == false);
    }

private:
    void CheckResult(const std::string& str, bool result) {
        std::cout << "Test for \"" << str << "\": " << (result ? "PASS" : "FAIL") << std::endl;
    }
};

int main() {
    RedixTrieTest tests;

    tests.TestInsertAndSearch();
    tests.TestSearchNonExistentString();
    tests.TestEmptyTrie();
    std::cout << "All tests passed!" << std::endl;

    return 0;
}
