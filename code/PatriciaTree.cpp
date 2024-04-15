#include "PatriciaTree.h"
#include <iostream>

void PatriciaTree::insert(const std::string &key) {
    insertRec(root, key, 0);
}

void PatriciaTree::insertRec(std::shared_ptr<PatriciaNode>& node, const std::string& key, size_t bitIndex) {
    if (bitIndex >= key.length() * 8) {
        node->isEndOfWord = true;
        return;
    }
    
    char bit = key[bitIndex / 8]; // Get the character containing the bit
    if (node->children.find(bit) == node->children.end()) {
        node->children[bit] = std::make_shared<PatriciaNode>(key.substr(bitIndex / 8));
    }
    
    insertRec(node->children[bit], key, bitIndex + 8); // Move to next character
}

bool PatriciaTree::find(const std::string &key) const {
    return findRec(root, key, 0);
}

bool PatriciaTree::findRec(std::shared_ptr<PatriciaNode>& node, const std::string& key, size_t bitIndex) const {
    if (bitIndex >= key.length() * 8) {
        return node->isEndOfWord;
    }
    
    char bit = key[bitIndex / 8];
    if (node->children.find(bit) == node->children.end()) {
        return false;
    }

    return findRec(node->children[bit], key, bitIndex + 8);
}
