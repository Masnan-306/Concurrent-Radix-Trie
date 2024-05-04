#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <mutex>
#include "RadixTrieLockFree.h"

template <typename O>
void searchForKey(RadixTreeParallel<O>& tree, const std::string& key, std::map<std::string, O>& results, std::mutex& resultsMutex) {
    try {
        O value = tree.getValueForExactKey(key);
        std::lock_guard<std::mutex> lock(resultsMutex);
        results[key] = value;
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
    }
}

int main() {
    RadixTreeParallel<int> tree;
    tree.put("apple", 1);
    tree.put("applestore", 1);
    tree.put("apricot", 2);
    tree.put("april", 2);
    tree.put("application", 1);
    tree.put("banana", 3);
    tree.put("berry", 4);
    tree.put("cherry", 5);

    // tree.print();

    // Map to hold expected results
    std::map<std::string, int> expectedResults = {
        {"apple", 1}, {"apricot", 2}, {"banana", 3}, {"berry", 4}, {"cherry", 5}, {"date", 0}, {"applestore", 1}, {"april", 2}, {"application", 1}
    };

    // Map to store actual results from threads
    std::map<std::string, int> actualResults;
    std::mutex resultsMutex;

    // List of keys to search for
    std::vector<std::string> keys = {"apple", "banana", "cherry", "date", "berry", "apricot", "applestore", "april", "application"};

    // Creating a vector to hold thread objects
    std::vector<std::thread> threads;

    // Launching multiple threads to perform searches
    for (const auto& key : keys) {
        threads.emplace_back(std::thread(searchForKey<int>, std::ref(tree), key, std::ref(actualResults), std::ref(resultsMutex)));
    }

    // Joining all threads to the main thread
    for (auto& th : threads) {
        th.join();
    }

    // Verify the results
    bool allCorrect = true;
    for (const auto& result : expectedResults) {
        if (actualResults[result.first] != result.second) {
            allCorrect = false;
            std::cout << "Error: Key '" << result.first << "' expected value " << result.second << " but found " << actualResults[result.first] << std::endl;
        }
    }

    if (allCorrect) {
        std::cout << "All thread results are correct.√√√√√√√√√√√" << std::endl;
    } else {
        std::cout << "Some thread results are incorrect.xxxxxxxxxxx" << std::endl;
    }

    return 0;
}
