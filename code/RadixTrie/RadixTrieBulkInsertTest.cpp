#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <omp.h>
#include "RadixTrieCoarseLock.h"

int main() {
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
    int numThreads = 1;
    omp_set_num_threads(numThreads);  // Using omp_set_num_threads to set thread count

    #pragma omp parallel for
    for (int i = 0; i < words.size(); i++) {
        trie.put(words[i], 1);
    }

    std::cout << "Inserted " << words.size() << " words into the Radix Trie using " << numThreads << " threads." << std::endl;

    std::cout << "Checking insertion of 'example': " << trie.getValueForExactKey("example") << std::endl;
    std::cout << "Checking insertion of 'aalii': " << trie.getValueForExactKey("aalii") << std::endl;
        std::cout << "Checking insertion of 'aaaaaaa': " << trie.getValueForExactKey("aaaaaaa") << std::endl;

    return 0;
}
