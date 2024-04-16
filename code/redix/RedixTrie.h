#ifndef PATRICIA_TREE_H
#define PATRICIA_TREE_H

#include <memory>
#include <string>
#include <vector>

class PatriciaNode {
public:
    std::string key;        // key fragment stored at this node
    bool isTerminal;        // true if this node represents the end of a key
    std::vector<std::shared_ptr<PatriciaNode>> children;  // children nodes

    explicit PatriciaNode(const std::string& key) : key(key), isTerminal(false) {}
};

class PatriciaTree {
private:
    std::shared_ptr<PatriciaNode> root;

    // Helper functions
    std::shared_ptr<PatriciaNode> insertRec(std::shared_ptr<PatriciaNode> node, const std::string& key);
    bool searchRec(const std::shared_ptr<PatriciaNode>& node, const std::string& key) const;

public:
    PatriciaTree() : root(std::make_shared<PatriciaNode>("")) {}

    void insert(const std::string& key);
    bool search(const std::string& key) const;
};

#endif // PATRICIA_TREE_H