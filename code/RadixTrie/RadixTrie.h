#ifndef RADIX_TRIE_H
#define RADIX_TRIE_H

#include <string>
#include <array>
#include <memory>
#include <vector>
#include <utility>
#include <iostream>

template <typename O>
class RadixNode {
public:
    std::string key;
    O value;
    std::array<std::shared_ptr<RadixNode<O>>, 26> children;
    bool isTerminal;

    explicit RadixNode(const std::string& key) : key(key), value(O()), isTerminal(false) {
        children.fill(nullptr);
    }
};

template <typename O>
class RadixTree {
private:
    std::shared_ptr<RadixNode<O>> root;

    std::shared_ptr<RadixNode<O>> insertRec(std::shared_ptr<RadixNode<O>> node, const std::string& key, const O& value, int depth = 0);
    std::shared_ptr<RadixNode<O>> findNode(const std::shared_ptr<RadixNode<O>>& node, const std::string& key, int depth = 0) const;
    std::vector<std::pair<std::string, O>> collectPairsRec(const std::shared_ptr<RadixNode<O>>& node, const std::string& prefix) const;

    // Helper function for printing the tree
    void printTree(const std::shared_ptr<RadixNode<O>>& node, const std::string& prefix, const std::string& childPrefix) const;

public:
    RadixTree() : root(std::make_shared<RadixNode<O>>("")) {}

    O put(const std::string& key, const O& value);
    O getValueForExactKey(const std::string& key) const;
    std::vector<std::string> getKeysStartingWith(const std::string& prefix) const;
    std::vector<O> getValuesForKeysStartingWith(const std::string& prefix) const;
    std::vector<std::pair<std::string, O>> collectPairs(const std::string& prefix) const;

    // Public method to print the entire trie
    void print() const;
};

#include "RadixTrieSequential.cpp"  // Include the implementation file

#endif // RADIX_TRIE_H
