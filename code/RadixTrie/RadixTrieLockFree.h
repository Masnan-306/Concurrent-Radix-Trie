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
    std::array<std::atomic<RadixNode<O>*>, 26> children;
    bool isTerminal;

    explicit RadixNode(const std::string& key, const O& value = O(), bool isTerminal = false);

    // Safely update the child pointer using compare and swap
    bool trySetChild(int index, RadixNode<O>* expected, RadixNode<O>* newValue);
    RadixNode<O>* getChild(int index) const;
};

template <typename O>
class RadixTree {
private:
    std::atomic<RadixNode<O>*> root;

public:
    RadixTree();

    // Lock-free insert method
    void put(const std::string& key, const O& value);
    bool insertRec(RadixNode<O>* node, const std::string& key, const O& value, int depth);
    O getValueForExactKey(const std::string& key);
};

#include "RadixTrieLockFree.tpp"

#endif // RADIX_TRIE_LOCK_FREE_H
