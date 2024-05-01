#ifndef Redix_TREE_H
#define Redix_TREE_H

#include <memory>
#include <string>
#include <vector>

class RedixNode {
public:
    std::string key;        // key fragment stored at this node
    bool isTerminal;        // true if this node represents the end of a key
    std::vector<std::shared_ptr<RedixNode>> children;  // children nodes

    explicit RedixNode(const std::string& key) : key(key), isTerminal(false) {}
};

class RedixTrie {
private:
    std::shared_ptr<RedixNode> root;

    // Helper functions
    std::shared_ptr<RedixNode> insertRec(std::shared_ptr<RedixNode> node, const std::string& key);
    bool searchRec(const std::shared_ptr<RedixNode>& node, const std::string& key) const;

public:
    RedixTrie() : root(std::make_shared<RedixNode>("")) {}

    void insert(const std::string& key);
    bool search(const std::string& key) const;
};

#endif // Redix_TREE_H