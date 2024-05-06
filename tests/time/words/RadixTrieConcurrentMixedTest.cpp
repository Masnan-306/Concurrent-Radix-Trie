#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <vector>
#include "code/RadixTrie/RadixTrieLockFree.h"

// Function to generate a random lowercase English letter
char generateRandomLetter() {
    return 'a' + rand() % 26;
}

// Function to generate a random lowercase English word of given length
std::string generateRandomWord(int length) {
    std::string word;
    for (int i = 0; i < length; ++i) {
        word += generateRandomLetter();
    }
    return word;
}

// Function to perform concurrent puts and getValueForExactKey operations based on PUT_RATIO
void performConcurrentOperations(RadixTreeParallel<int>& trie, double putRatio, int numWords) {
    #pragma omp parallel for
    for (int i = 0; i < numWords; ++i) {
        double randNum = (double) rand() / RAND_MAX;
        if (randNum < putRatio) {
            std::string key = generateRandomWord(10);
            trie.put(key, i);
        } else {
            std::string key = generateRandomWord(10);
            trie.getValueForExactKey(key);
        }
    }
}

int main() {
    const int NUM_THREADS = 2; // Number of concurrent threads
    const int PROBLEM_SIZE = 10000000; // Number of words generated
    const double PUT_RATIO = 0.5; // Ratio of puts

    // Set number of threads for OpenMP
    omp_set_num_threads(NUM_THREADS);

    // Initialize Radix Trie
    RadixTreeParallel<int> trie;

    // Perform concurrent puts and getValueForExactKey operations
    std::cout << "Performing concurrent operations..." << std::endl;
    double startTime = omp_get_wtime();
    performConcurrentOperations(trie, PUT_RATIO, PROBLEM_SIZE);
    double endTime = omp_get_wtime();
    std::cout << "Concurrent operations took " << endTime - startTime << " seconds." << std::endl;

    return 0;
}