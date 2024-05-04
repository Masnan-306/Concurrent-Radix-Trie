// RadixTrieTest.cpp
#include "code/RadixTrie/RadixTrieLockFree.h"
#include <iostream>
#include <thread>
#include <vector>
#include <map>

void testInsertAndSearch() {
    RadixTreeParallel<int> tree;
    tree.put("apple", 10);
    bool test1 = (tree.getValueForExactKey("apple") == 10);
    bool test2 = (tree.getValueForExactKey("app") == int());

    std::cout << "Test Insert and Search: "
              << (test1 && test2 ? "PASSED" : "FAILED!!!") << std::endl;
}


// void testCollectPairs() {
//     RadixTree<int> tree;
//     tree.put("apple", 10);
//     tree.put("app", 5);
//     tree.put("ape", 7);

//     auto pairs = tree.collectPairs("ap");
//     bool test = (pairs.size() == 3);

//     std::cout << "Test Collect Pairs: "
//               << (test ? "PASSED" : "FAILED!!!") << std::endl;
// }

// void testGetKeysStartingWith() {
//     RadixTree<int> tree;
//     tree.put("apple", 10);
//     tree.put("app", 5);

//     auto keys = tree.getKeysStartingWith("app");
//     bool test = (keys.size() == 2 && find(keys.begin(), keys.end(), "apple") != keys.end() && find(keys.begin(), keys.end(), "app") != keys.end());

//     std::cout << "Test Get Keys Starting With: "
//               << (test ? "PASSED" : "FAILED!!!") << std::endl;
// }

// void testNoPairs() {
//     RadixTree<int> tree;
//     tree.put("apple", 10);
//     tree.put("app", 5);
//     tree.put("ape", 7);

//     auto pairs = tree.collectPairs("bp");
//     bool test = (pairs.size() == 0);

//     std::cout << "Test Collect 0 Pairs: "
//               << (test ? "PASSED" : "FAILED!!!") << std::endl;
// }

void testUpdateExistingKey() {
    RadixTreeParallel<int> tree;
    tree.put("apple", 10);
    tree.put("apple", 20);

    bool test = (tree.getValueForExactKey("apple") == 20);

    std::cout << "Test Update Existing Key: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

void testSearchNonExistingKey() {
    RadixTreeParallel<int> tree;
    tree.put("apple", 10);

    bool test = (tree.getValueForExactKey("orange") == int());

    std::cout << "Test Search Non-Existing Key: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

void testEdgeCases() {
    RadixTreeParallel<int> tree;
    tree.put("i", 0);
    tree.put("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1);

    bool test1 = (tree.getValueForExactKey("i") == 0);
    bool test2 = (tree.getValueForExactKey("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") == 1);
    bool test3 = (tree.getValueForExactKey("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa") == 0);

    std::cout << "Test Edge Cases: "
              << (test1 && test2 && test3 ? "PASSED" : "FAILED!!!") << std::endl;
}

void testSearchExistingLongerKey() {
    RadixTreeParallel<int> tree;
    tree.put("apple", 10);
    tree.put("applet", 9);

    bool test = (tree.getValueForExactKey("applet") == 9);

    // tree.print();

    std::cout << "Test Search for Insert in Longer Existing Key: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

void testReorderedPuts() {
    RadixTreeParallel<int> tree;
    tree.put("app", 9);
    tree.put("ape", 9);
    tree.put("apple", 10);

    // tree.print();

    bool test = (tree.getValueForExactKey("app") == 9);

    std::cout << "Test Search for Reordered Inserts: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

// ---------------------------------------
// Concurrent Tests Begin Here
// ---------------------------------------

void threadPutTask(RadixTreeParallel<int>& tree, const std::string& key, int value) {
    tree.put(key, value);
    // std::cout << "Inserted (" << key << ", " << value << ")\n";
}

void testConcurrentPuts() {
    RadixTreeParallel<int> tree;
    std::vector<std::thread> threads;

    // Start multiple threads to insert different keys
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "apple", 10));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "app", 5));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "ape", 7));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "banana", 20));

    for (auto& thread : threads) {
        thread.join();
    }

    // tree.print();

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

void testConcurrentRace() {
    RadixTreeParallel<int> tree;
    std::vector<std::thread> threads;

    // Start multiple threads to insert different keys
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "appla", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "apa", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "applb", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "apb", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "applc", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "apc", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "appld", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "apd", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "apple", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "ape", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "applf", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "apf", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "applg", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "apg", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "aiami", 1));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "aimmi", 1));

    for (auto& thread : threads) {
        thread.join();
    }

    // tree.print();

    // Test consistency after all threads have finished
    bool test = (tree.getValueForExactKey("appla") == 1) &&
                (tree.getValueForExactKey("applb") == 1) &&
                (tree.getValueForExactKey("applc") == 1) &&
                (tree.getValueForExactKey("appld") == 1) &&
                (tree.getValueForExactKey("apple") == 1) &&
                (tree.getValueForExactKey("applf") == 1) &&
                (tree.getValueForExactKey("applg") == 1) &&
                (tree.getValueForExactKey("apa") == 1) &&
                (tree.getValueForExactKey("apb") == 1) &&
                (tree.getValueForExactKey("apc") == 1) &&
                (tree.getValueForExactKey("apd") == 1) &&
                (tree.getValueForExactKey("ape") == 1) &&
                (tree.getValueForExactKey("apf") == 1) &&
                (tree.getValueForExactKey("aiami") == 1) &&
                (tree.getValueForExactKey("aimmi") == 1) &&
                (tree.getValueForExactKey("apg") == 1);

    std::cout << "Test Concurrent Puts: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
    
}

void testConcurrentOverlappingPrefixes() {
    RadixTreeParallel<int> tree;
    std::vector<std::thread> threads;

    // Insert keys with overlapping prefixes
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "apple", 10));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "applet", 15));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "application", 20));
    threads.push_back(std::thread(threadPutTask, std::ref(tree), "apparatus", 25));

    for (auto& thread : threads) {
        thread.join();
    }

    // Verification
    bool test = (tree.getValueForExactKey("apple") == 10) &&
                (tree.getValueForExactKey("applet") == 15) &&
                (tree.getValueForExactKey("application") == 20) &&
                (tree.getValueForExactKey("apparatus") == 25);

    // std::cout << tree.getValueForExactKey("apple") << std::endl;
    // std::cout << tree.getValueForExactKey("applet") << std::endl;
    // std::cout << tree.getValueForExactKey("application")<< std::endl;
    // std::cout << tree.getValueForExactKey("apparatus") << std::endl;
    
    std::cout << "Test Concurrent Overlapping Prefixes: "
              << (test ? "PASSED" : "FAILED!!!") << std::endl;
}

void testHighVolumeConcurrentAccess() {
    RadixTreeParallel<int> tree;
    std::vector<std::thread> threads;
    int numThreads = 100;  // Example scale
    std::vector<std::string> keys = {"node", "note", "notion", "network", "net", "neutron", "newton"};
    std::map<std::string, int> lastValues;

    // Prepare last expected values map
    for (int i = 0; i < numThreads; ++i) {
        std::string key = keys[i % keys.size()];
        lastValues[key] = i;  // Since the last thread setting the value will be the last value for each key
    }

    for (int i = 0; i < numThreads; ++i) {
        std::string key = keys[i % keys.size()];
        threads.push_back(std::thread(threadPutTask, std::ref(tree), key, i));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Verification of consistency and correctness
    bool testPassed = true;
    for (auto& pair : lastValues) {
        if (tree.getValueForExactKey(pair.first) != pair.second) {
            testPassed = false;
            std::cout << pair.first << std::endl;
            break;
        }
    }

    std::cout << "Test High Volume Concurrent Access: "
              << (testPassed ? "PASSED" : "FAILED!!!") << std::endl;
}


void testStaggeredInserts() {
    RadixTreeParallel<int> tree;
    std::vector<std::thread> threads;

    threads.push_back(std::thread([](RadixTreeParallel<int>& tree){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        threadPutTask(tree, "alpha", 100);
    }, std::ref(tree)));

    threads.push_back(std::thread([](RadixTreeParallel<int>& tree){
        threadPutTask(tree, "omega", 200);
    }, std::ref(tree)));

    for (auto& thread : threads) {
        thread.join();
    }

    bool testPassed = (tree.getValueForExactKey("alpha") == 100) &&
                      (tree.getValueForExactKey("omega") == 200);

    std::cout << "Test Staggered Inserts: "
              << (testPassed ? "PASSED" : "FAILED!!!") << std::endl;
}


int main() {
    
    // testCollectPairs();
    // testGetKeysStartingWith();
    // testNoPairs();

    testInsertAndSearch();
    testUpdateExistingKey();
    testSearchNonExistingKey();
    testEdgeCases();
    testSearchExistingLongerKey();
    testReorderedPuts();

    // Concurrent Tests
    testConcurrentPuts();
    testConcurrentRace();
    testConcurrentOverlappingPrefixes();
    testHighVolumeConcurrentAccess();
    testStaggeredInserts();
    return 0;
}
