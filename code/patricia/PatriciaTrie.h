// PatriciaTree.h
#ifndef PATRICIATREE_H
#define PATRICIATREE_H

#include <memory>
#include <string>

class PatriciaNode {
public:
    std::string key;
    void* value; // Using void* for generic value pointer. Should be templated in a real implementation.
    std::shared_ptr<PatriciaNode> left, right;
    int diff; // Difference position in the key

    PatriciaNode(std::string key, void* value, int diff);
};

class PatriciaTree {
private:
    std::shared_ptr<PatriciaNode> root;

    std::shared_ptr<PatriciaNode> createNode(const std::string& key, void* value, int diff);
    int computeDiff(const std::shared_ptr<PatriciaNode>& node1, const std::string& key);
    int bitAtPosition(const std::string& key, int position);

public:
    PatriciaTree();
    bool insert(const std::string& key, void* value);
    std::shared_ptr<PatriciaNode> search(const std::string& key);
};

#endif // PATRICIATREE_H