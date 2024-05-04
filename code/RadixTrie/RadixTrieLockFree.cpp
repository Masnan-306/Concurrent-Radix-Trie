// RadixTrie.cpp
#include "RadixTrie.h"
#include <iostream>

template <typename O>
std::shared_ptr<RadixNode<O>> RadixTree<O>::insertRec(std::shared_ptr<RadixNode<O>> node, const std::string& key, const O& value, int depth) {
}

template <typename O>
O RadixTree<O>::put(const std::string& key, const O& value) {
    
}

template <typename O>
std::shared_ptr<RadixNode<O>> RadixTree<O>::findNode(const std::shared_ptr<RadixNode<O>>& node, const std::string& key, int depth) const {
    
}

template <typename O>
O RadixTree<O>::getValueForExactKey(const std::string& key) const {
    
}

template <typename O>
std::vector<std::pair<std::string, O>> RadixTree<O>::collectPairs(const std::string& prefix) const {
    
}

template <typename O>
std::vector<std::pair<std::string, O>> RadixTree<O>::collectPairsRec(const std::shared_ptr<RadixNode<O>>& node, const std::string& prefix) const {
    
}

template <typename O>
std::vector<std::string> RadixTree<O>::getKeysStartingWith(const std::string& prefix) const {
    
}

template <typename O>
std::vector<O> RadixTree<O>::getValuesForKeysStartingWith(const std::string& prefix) const {
    
}

template <typename O>
void RadixTree<O>::printTree(const std::shared_ptr<RadixNode<O>>& node, const std::string& prefix, const std::string& childPrefix) const {
    
}

template <typename O>
void RadixTree<O>::print() const {
    
}
