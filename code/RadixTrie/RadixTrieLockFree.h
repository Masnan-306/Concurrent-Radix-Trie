#ifndef RADIX_TRIE_LOCK_FREE_H
#define RADIX_TRIE_LOCK_FREE_H

#include <atomic>
#include <array>
#include <memory>
#include <string>

template <typename O>
class RadixNode {
public:
    std::string key;
    O value;
    bool isTerminal;
    std::shared_ptr<std::array<std::atomic<RadixNode<O>*>, 26>> children;

    explicit RadixNode(const std::string& key, const O& value = O(), bool isTerminal = false);

    // Safely update the child pointer using compare and swap
    bool trySetChild(int index, RadixNode<O>* expected, RadixNode<O>* newValue);
    RadixNode<O>* getChild(int index) const;
};

template <typename O>
class RadixTreeParallel {
private:
    std::atomic<RadixNode<O>*> root;
    void printTree(RadixNode<O>* node, const std::string& prefix, const std::string& childPrefix) const;
    bool insertHelper(RadixNode<O>* node, const std::string& key, const O& value);
    void collectPairsDFS(RadixNode<O>* node, const std::string& prefix, std::vector<std::pair<std::string, O>>& pairs) const;

public:
    RadixTreeParallel() : root(new RadixNode<O>("")) {}

    // Lock-free insert method
    void put(const std::string& key, const O& value);
    O getValueForExactKey(const std::string& key);
    std::vector<std::pair<std::string, O>> collectPairs(const std::string& prefix) const;
    void print() const;
};

#include "RadixTrieLockFree.tpp"

#endif // RADIX_TRIE_LOCK_FREE_H
