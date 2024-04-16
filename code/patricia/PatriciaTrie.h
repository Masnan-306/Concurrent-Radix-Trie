#ifndef PATRICIATREE_H
#define PATRICIATREE_H

#include <iostream>
#include <string>
#include <bitset>
#include <memory>

class PatriciaNode {
public:
    std::string key;
    std::string value;
    std::shared_ptr<PatriciaNode> left, right;
    int diff;

    PatriciaNode(std::string key, std::string value, int diff)
        : key(key), value(value), diff(diff), left(nullptr), right(nullptr) {}
};

class PatriciaTree {
private:
    std::shared_ptr<PatriciaNode> root;

    std::shared_ptr<PatriciaNode> createNode(const std::string& key, const std::string& value, int diff);
    int computeDiff(const std::shared_ptr<PatriciaNode>& node1, const std::string& key);
    int bitAtPosition(const std::string& key, int position);
    std::shared_ptr<PatriciaNode> searchHelper(const std::string& key);
    bool insertHelper(const std::string& key, const std::string& value);
    void printNode(const std::shared_ptr<PatriciaNode>& node, const std::string& indent);

public:
    PatriciaTree();
    bool insert(const std::string& s);
    bool search(const std::string& key);
    void printTrie();
};

std::string stringToBinary(const std::string& input);

#endif /* PATRICIATREE_H */
