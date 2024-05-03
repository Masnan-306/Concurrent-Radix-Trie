#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <omp.h>
#include <chrono>

#include "RadixTrieLockFineLock.h"

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

    // Set the number of threads to use
    int numThreads = 2;
    omp_set_num_threads(numThreads);  // Using omp_set_num_threads to set thread count
    
    const double init_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - init_start).count();
    std::cout << "Initialization time (sec): " << std::fixed << std::setprecision(10) << init_time << '\n';

  const auto compute_start = std::chrono::steady_clock::now();
    #pragma omp parallel for
    for (int i = 0; i < words.size(); i++) {
        for (int j = 0; j< 10; j++){
            trie.put(words[i], 1);
        }
    }

    std::cout << "Inserted " << words.size() << " words into the Radix Trie using " << numThreads << " threads." << std::endl;

    std::cout << "Checking insertion of 'example': " << trie.getValueForExactKey("example") << std::endl;
    std::cout << "Checking insertion of 'aalii': " << trie.getValueForExactKey("aalii") << std::endl;
    std::cout << "Checking insertion of 'aaaaaaa': " << trie.getValueForExactKey("aaaaaaa") << std::endl;
    const double compute_time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - compute_start).count();
    std::cout << "Computation time (sec): " << compute_time << '\n';
    return 0;
}
