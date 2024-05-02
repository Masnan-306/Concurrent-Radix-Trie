#ifndef RADIX_TRIE_LOCK_H
#define RADIX_TRIE_LOCK_H

#include <mutex>
#include "RadixTrie.h"

template <typename O>
class RadixTreeLock {
private:
    RadixTree<O> tree;
    mutable std::mutex mtx;

public:
    RadixTreeLock() = default;

    O put(const std::string& key, const O& value) {
        std::lock_guard<std::mutex> lock(mtx);
        return tree.put(key, value);
    }

    O getValueForExactKey(const std::string& key) const {
        std::lock_guard<std::mutex> lock(mtx);
        return tree.getValueForExactKey(key);
    }

    std::vector<std::string> getKeysStartingWith(const std::string& prefix) const {
        std::lock_guard<std::mutex> lock(mtx);
        return tree.getKeysStartingWith(prefix);
    }

    std::vector<O> getValuesForKeysStartingWith(const std::string& prefix) const {
        std::lock_guard<std::mutex> lock(mtx);
        return tree.getValuesForKeysStartingWith(prefix);
    }

    std::vector<std::pair<std::string, O>> collectPairs(const std::string& prefix) const {
        std::lock_guard<std::mutex> lock(mtx);
        return tree.collectPairs(prefix);
    }

    void print() const {
        std::lock_guard<std::mutex> lock(mtx);
        tree.print();
    }
};

#endif // RADIX_TRIE_LOCK_H
