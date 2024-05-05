#ifndef RADIX_TRIE_LOCK_H
#define RADIX_TRIE_LOCK_H

#include <mutex>
#include <array>
#include <memory>
#include <string>
#include <vector>  // Required for std::vector
#include <stack>   // Required for std::stack

template <typename O>
class RadixNode {
public:
    std::string key;
    O value;
    std::array<RadixNode<O>*, 26> children;
    bool isTerminal;
    std::mutex nodeMutex;

    explicit RadixNode(const std::string& key, const O& value = O(), bool isTerminal = false) : 
        key(key), value(value), isTerminal(isTerminal) {
        children.fill(nullptr);
    }
};

template <typename O>
class RadixTreeParallel {
private:
    RadixNode<O>* root;
    void insertIterative(RadixNode<O>* node, const std::string& key, const O& value);
    void printTree(RadixNode<O>* node, const std::string& prefix, const std::string& childPrefix) const;
    void collectPairsDFS(RadixNode<O>* node, const std::string& prefix, std::vector<std::pair<std::string, O>>& pairs) const;
    void testAndCoalesceChild(RadixNode<O>* node, RadixNode<O>* parent);

public:
    RadixTreeParallel() : root(new RadixNode<O>("")) {}

    // Thread-safe insert method
    void put(const std::string& key, const O& value);

    // Thread-safe method to retrieve the value for a given key
    O getValueForExactKey(const std::string& key);

    // Thread-safe method to retrieve all key-value pairs where prefix is in the key
    std::vector<std::pair<std::string, O>> collectPairs(const std::string& prefix) const;

    void removeKey(const std::string& key);

    void print() const;
};

#include "RadixTrieFineLock.cpp"

#endif // RADIX_TRIE_LOCK_H
