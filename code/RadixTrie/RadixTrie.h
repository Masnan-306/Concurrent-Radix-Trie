#ifndef RADIX_TREE_H
#define RADIX_TREE_H

#include <string>
#include <array>
#include <memory>
#include <vector>
#include <utility>

template <typename O>
class RadixNode {
public:
    std::string key;  // Key fragment stored at this node
    O value;  // Single value associated with this node
    std::array<std::shared_ptr<RadixNode<O>>, 26> children;  // Array of children nodes, one for each letter a-z
    bool isTerminal;  // True if this node represents the end of a key

    explicit RadixNode(const std::string& key) : key(key), value(O()), isTerminal(false) {
        children.fill(nullptr);  // Initialize all child pointers to nullptr
    }
};

template <typename O>
class RadixTree {
private:
    std::shared_ptr<RadixNode<O>> root;

    // Helper functions
    std::shared_ptr<RadixNode<O>> insertRec(std::shared_ptr<RadixNode<O>> node, const std::string& key, const O& value);
    std::shared_ptr<RadixNode<O>> findNode(const std::shared_ptr<RadixNode<O>>& node, const std::string& key) const;
    std::vector<std::pair<std::string, O>> collectPairsRec(const std::shared_ptr<RadixNode<O>>& node, const std::string& prefix) const;

public:
    RadixTree() : root(std::make_shared<RadixNode<O>>("")) {}

    O put(const std::string& key, const O& value);
    O getValueForExactKey(const std::string& key) const;

    std::vector<std::string> getKeysStartingWith(const std::string& prefix) const;
    std::vector<O> getValuesForKeysStartingWith(const std::string& prefix) const;
    std::vector<std::pair<std::string, O>> (const std::string& prefix) const;

    int size() const;
};