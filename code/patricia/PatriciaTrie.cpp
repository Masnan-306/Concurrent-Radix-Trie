// PatriciaTree.cpp
#include "PatriciaTree.h"

PatriciaNode::PatriciaNode(std::string key, void* value, int diff)
    : key(key), value(value), diff(diff), left(nullptr), right(nullptr) {}

int PatriciaTree::computeDiff(const std::shared_ptr<PatriciaNode>& node1, const std::string& key) {
    // Implementation of ComputeDiff
    // Placeholder for actual bit comparison logic
    return 0;
}

int PatriciaTree::bitAtPosition(const std::string& key, int position) {
    // Implementation of BitAtPosition
    // Placeholder for actual bit extraction logic
    return 0;
}

std::shared_ptr<PatriciaNode> PatriciaTree::createNode(const std::string& key, void* value, int diff) {
    return std::make_shared<PatriciaNode>(key, value, diff);
}

PatriciaTree::PatriciaTree() : root(nullptr) {}

bool PatriciaTree::insert(const std::string& key, void* value) {
    if (root == nullptr) {
        root = createNode(key, value, 0);
        return true;
    }

    auto found = search(key);
    if (found != nullptr && found->key == key) {
        found->value = value;
        return true;
    }

    auto parent = root;
    auto child = root->left;
    int diff = computeDiff(nullptr, key);

    while (parent->diff < child->diff && diff < child->diff) {
        parent = child;
        int bit = bitAtPosition(key, diff);
        child = (bit == 0) ? child->left : child->right;
    }

    auto node = createNode(key, value, diff);
    int bit = bitAtPosition(key, diff);

    if (bit == 0) {
        node->left = node;
        node->right = child;
    } else {
        node->left = child;
        node->right = node;
    }

    if (bitAtPosition(parent->key, diff) == 0) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    return true;
}

std::shared_ptr<PatriciaNode> PatriciaTree::search(const std::string& key) {
    if (root == nullptr || root->key == key) {
        return root;
    }

    auto child = root->left;
    while (child->diff < child->left->diff || child->diff < child->right->diff) {
        int bit = bitAtPosition(key, child->diff);
        child = (bit == 0) ? child->left : child->right;
    }

    if (child->key == key) {
        return child;
    }

    return nullptr;
}