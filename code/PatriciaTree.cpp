#include "PatriciaTree.h"

void PatriciaTree::insert(const std::string &key) {
    root = insertRec(root, key);
}

std::shared_ptr<PatriciaNode> PatriciaTree::insertRec(std::shared_ptr<PatriciaNode> node, const std::string &key) {
    if (node == nullptr) {
        auto newNode = std::make_shared<PatriciaNode>(key);
        newNode->isTerminal = true;
        return newNode;
    }

    // Find the longest common prefix
    int commonLength = 0;
    int minLen = std::min(node->key.length(), key.length());
    while (commonLength < minLen && node->key[commonLength] == key[commonLength]) {
        commonLength++;
    }

    if (commonLength == node->key.length()) {
        // The entire key of the node is a prefix of the inserted key
        if (commonLength == key.length()) {
            node->isTerminal = true; // The whole key matches the node key
        } else {
            std::string newKey = key.substr(commonLength);
            node->children.push_back(insertRec(nullptr, newKey));
        }
    } else {
        // Split the node
        auto splitNode = std::make_shared<PatriciaNode>(node->key.substr(commonLength));
        splitNode->children = std::move(node->children);
        splitNode->isTerminal = node->isTerminal;

        node->key = key.substr(0, commonLength);
        node->children.clear();
        node->children.push_back(splitNode);

        if (commonLength == key.length()) {
            node->isTerminal = true;
        } else {
            std::string newKey = key.substr(commonLength);
            node->children.push_back(insertRec(nullptr, newKey));
        }
        node->isTerminal = false;
    }

    return node;
}

bool PatriciaTree::search(const std::string& key) const {
    return searchRec(root, key);
}

bool PatriciaTree::searchRec(const std::shared_ptr<PatriciaNode>& node, const std::string& key) const {
    if (node == nullptr) return false;
    
    int commonLength = 0;
    int minLen = std::min(node->key.length(), key.length());
    while (commonLength < minLen && node->key[commonLength] == key[commonLength]) {
        commonLength++;
    }

    if (commonLength == key.length() && commonLength == node->key.length() && node->isTerminal) {
        return true; // Full match
    }

    if (commonLength < node->key.length() || commonLength == key.length()) {
        return false; // Partial match, but not complete
    }

    std::string newKey = key.substr(commonLength);
    for (auto& child : node->children) {
        if (searchRec(child, newKey)) {
            return true;
        }
    }

    return false;
}
