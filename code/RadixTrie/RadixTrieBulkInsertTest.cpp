#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <omp.h>
#include <chrono>
#include <random>

#include "RadixTrieLockFree.h"

int main() {
    const auto init_start = std::chrono::steady_clock::now();
    RadixTreeParallel<int> trie;
    std::ifstream file("../../tests/time/words/hard_filtered_words.txt");
    std::vector<std::string> words;

    if (!file) {
        std::cerr << "Unable to open the file hard_filtered_words.txt" << std::endl;
        return 1;
    }

    std::string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            words.push_back(line);
        }
    }
    file.close();

    int numThreads = 4;
    omp_set_num_threads(numThreads);

    const double init_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - init_start).count();
    std::cout << "Initialization time (sec): " << std::fixed << std::setprecision(10) << init_time << '\n';

    const auto compute_start = std::chrono::steady_clock::now();
    #pragma omp parallel for schedule(dynamic)
    for (int j = 0; j< 30; j++) {
        for (int i = 0; i < words.size(); i++) {
            trie.put(words[i], 1);
        }
    }

    #pragma omp barrier

    std::cout << "Inserted " << words.size() << " words into the Radix Trie using " << numThreads << " threads." << std::endl;

    // Random test access
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, words.size() - 1);

    const int numTests = 100000; // Number of random tests
    int failedCount = 0;

    #pragma omp parallel for reduction(+:failedCount) schedule(dynamic)
    for (int i = 0; i < numTests; i++) {
        int index = distrib(gen);
        // trie.getValueForExactKey(words[index]);
        if (trie.getValueForExactKey(words[index]) != 1) {
            std::cerr << "Test failed for word: " << words[index] << std::endl;
            failedCount++;
        }
    }

    std::cout << "Performed " << numTests << " random access tests, with " << failedCount << " failures." << std::endl;
    // trie.print();
    const double compute_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - compute_start).count();
    std::cout << "Computation time (sec): " << compute_time << '\n';

    return 0;
}
