#ifndef Radix_TREE_H
#define Radix_TREE_H

#include <memory>
#include <string>
#include <vector>

class RadixNode {
public:
    std::string key;        // key fragment stored at this node
    bool isTerminal;        // true if this node represents the end of a key
    std::vector<std::shared_ptr<RadixNode>> children;  // children nodes

    explicit RadixNode(const std::string& key) : key(key), isTerminal(false) {}
};

class RadixTrie {
private:
    std::shared_ptr<RadixNode> root;

    // Helper functions
    std::shared_ptr<RadixNode> insertRec(std::shared_ptr<RadixNode> node, const std::string& key);
    bool searchRec(const std::shared_ptr<RadixNode>& node, const std::string& key) const;
    void removeRec(std::shared_ptr<RadixNode>& node, const std::string& key);

public:
    RadixTrie() : root(std::make_shared<RadixNode>("")) {}

    void insert(const std::string& key);
    bool search(const std::string& key) const;
    void remove(const std::string& key);
    std::vector<std::string> prefixSearch(const std::string& prefix) const;
};

#endif // Radix_TREE_H