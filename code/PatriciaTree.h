#ifndef PATRICIA_TREE_H
#define PATRICIA_TREE_H

#include <memory>
#include <string>
#include <unordered_map>

class PatriciaNode {
public:
    bool isEndOfWord;
    std::string key; // the key fragment stored at this node
    std::unordered_map<char, std::shared_ptr<PatriciaNode>> children;

    PatriciaNode(const std::string &key) : key(key), isEndOfWord(false) {}
};

class PatriciaTree {
private:
    std::shared_ptr<PatriciaNode> root;

    void insertRec(std::shared_ptr<PatriciaNode>& node, const std::string& key, size_t bitIndex);
    bool findRec(std::shared_ptr<PatriciaNode>& node, const std::string& key, size_t bitIndex) const;

public:
    PatriciaTree() {
        root = std::make_shared<PatriciaNode>("");
    }

    void insert(const std::string &key);
    bool find(const std::string &key) const;
};

#endif // PATRICIA_TREE_H
