// RadixTrieTest.cpp
#include "RadixTrieCoarseLock.h"
#include <iostream>
#include <thread>
#include <vector>

void testInsertAndSearch() {
    RadixTree<int> tree;
    tree.put("apple", 10);

    bool test1 = (tree.getValueForExactKey("apple") == 10);
    bool test2 = (tree.getValueForExactKey("app") == int());

    std::cout << "Test Insert and Search: "
              << (test1 && test2 ? "PASSED" : "FAILED!!!") << std::endl;
}

void testCollectPairs() {
    RadixTree<int> tree;
    tree.put("apple", 10);
    tree.put("app", 5);
    tree.put("ape", 7);

    auto pairs = tree.collectPairs("ap");
    bool test = (pairs.size() == 3);

    std::cout << "Test Collect Pairs: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

void testGetKeysStartingWith() {
    RadixTree<int> tree;
    tree.put("apple", 10);
    tree.put("app", 5);

    auto keys = tree.getKeysStartingWith("app");
    bool test = (keys.size() == 2 && find(keys.begin(), keys.end(), "apple") != keys.end() && find(keys.begin(), keys.end(), "app") != keys.end());

    std::cout << "Test Get Keys Starting With: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

void testNoPairs() {
    RadixTree<int> tree;
    tree.put("apple", 10);
    tree.put("app", 5);
    tree.put("ape", 7);

    auto pairs = tree.collectPairs("bp");
    bool test = (pairs.size() == 0);

    std::cout << "Test Collect 0 Pairs: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

void testUpdateExistingKey() {
    RadixTree<int> tree;
    tree.put("apple", 10);
    tree.put("apple", 20);

    bool test = (tree.getValueForExactKey("apple") == 20);

    std::cout << "Test Update Existing Key: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

void testSearchNonExistingKey() {
    RadixTree<int> tree;
    tree.put("apple", 10);

    bool test = (tree.getValueForExactKey("orange") == int());

    std::cout << "Test Search Non-Existing Key: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

void testEdgeCases() {
    RadixTree<int> tree;
    tree.put("", 0);
    tree.put("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1);

    bool test1 = (tree.getValueForExactKey("") == 0);
    bool test2 = (tree.getValueForExactKey("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") == 1);
    bool test3 = (tree.getValueForExactKey("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa") == 0);

    std::cout << "Test Edge Cases: "
              << (test1 && test2 && test3 ? "PASSED" : "FAILED!!!") << std::endl;
}

void testSearchExistingLongerKey() {
    RadixTree<int> tree;
    tree.put("apple", 10);
    tree.put("app", 9);

    bool test = (tree.getValueForExactKey("app") == 9);

    std::cout << "Test Search for Insert in Longer Existing Key: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

void testReorderedPuts() {
    RadixTree<int> tree;
    tree.put("app", 9);
    tree.put("ape", 9);
    tree.put("apple", 10);

    bool test = (tree.getValueForExactKey("app") == 9);

    std::cout << "Test Search for Reordered Inserts: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

// ---------------------------------------
// Concurrent Tests Begin Here
// ---------------------------------------

void threadPutTask(RadixTreeLock<int>& tree, const std::string& key, int value) {
    tree.put(key, value);
    // std::cout << "Inserted (" << key << ", " << value << ")\n";
}

void testConcurrentPuts() {
    RadixTreeLock<int> tree;
    std::vector<std::thread> threads;

    // Start multiple threads to insert different keys
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "apple", 10));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "app", 5));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "ape", 7));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "banana", 20));

    for (auto& thread : threads) {
        thread.join();
    }

    // Test consistency after all threads have finished
    bool test = (tree.getValueForExactKey("apple") == 10) &&
                (tree.getValueForExactKey("app") == 5) &&
                (tree.getValueForExactKey("ape") == 7) &&
                (tree.getValueForExactKey("banana") == 20);

    std::cout << "Test Concurrent Puts: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
    
    // tree.print();
    // std::cout << "Test Concurrency: " << tree.getValueForExactKey("apple") << std::endl;
    // std::cout << "Test Concurrency: " << tree.getValueForExactKey("app") << std::endl;
    // std::cout << "Test Concurrency: " << tree.getValueForExactKey("ape") << std::endl;
    // std::cout << "Test Concurrency: " << tree.getValueForExactKey("banana") << std::endl;
}

int main() {
    testInsertAndSearch();
    testCollectPairs();
    testGetKeysStartingWith();
    testNoPairs();
    testUpdateExistingKey();
    testSearchNonExistingKey();
    testEdgeCases();
    testSearchExistingLongerKey();
    testReorderedPuts();

    // Concurrent Tests
    testConcurrentPuts();
    return 0;
}
