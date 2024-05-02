#include <atomic>
#include <array>
#include <memory>
#include <string>
#include <iostream>

// RadixNode implementation
template <typename O>
RadixNode<O>::RadixNode(const std::string& key, const O& value, bool isTerminal) : key(key), value(value), isTerminal(isTerminal) {
    for (auto& child : children) {
        child.store(nullptr, std::memory_order_relaxed);
    }
}

template <typename O>
bool RadixNode<O>::trySetChild(int index, RadixNode<O>* expected, RadixNode<O>* newValue) {
    return children[index].compare_exchange_strong(expected, newValue,
                                                   std::memory_order_release,
                                                   std::memory_order_relaxed);
}

template <typename O>
RadixNode<O>* RadixNode<O>::getChild(int index) const {
    return children[index].load(std::memory_order_acquire);
}

// RadixTree implementation
template <typename O>
RadixTreeParallel<O>::RadixTreeParallel() : root(nullptr) {}

// Lock-free insert method
template <typename O>
void RadixTreeParallel<O>::put(const std::string& key, const O& value) {
    RadixNode<O>* newNode = new RadixNode<O>(key, value, true);

    while (true) {
        RadixNode<O>* currentRoot = root.load(std::memory_order_acquire);
        if (!currentRoot) {
            if (root.compare_exchange_weak(currentRoot, newNode, std::memory_order_release, std::memory_order_relaxed)) {
                return;
            }
        } else {
            if (insertRec(currentRoot, key, value, 0)) {
                delete newNode; // If insertion is successful, delete the created node
                return;
            }
        }
    }
}

template <typename O>
bool RadixTreeParallel<O>::insertRec(RadixNode<O>* node, const std::string& key, const O& value, int depth) {
    if (!node) {
        return false;
    }

    // Compute common prefix length
    int commonPrefixLength = 0;
    while (commonPrefixLength < node->key.length() && commonPrefixLength + depth < key.length() &&
            node->key[commonPrefixLength] == key[depth + commonPrefixLength]) {
        ++commonPrefixLength;
    }

    // Split node if necessary
    if (commonPrefixLength < node->key.length()) {
        // Create a new node for the split part
        RadixNode<O>* splitNode = new RadixNode<O>(node->key.substr(commonPrefixLength), node->value, node->isTerminal);

        // Copy each child atomically
        for (size_t i = 0; i < node->children.size(); i++) {
            RadixNode<O>* child = node->children[i].load(std::memory_order_acquire);
            if (child) {
                splitNode->children[i].store(child, std::memory_order_release);
            }
        }

        // Create a new parent node for the part before the split
        RadixNode<O>* newParentNode = new RadixNode<O>(node->key.substr(0, commonPrefixLength));
        int index = node->key[commonPrefixLength] - 'a';
        newParentNode->children[index].store(splitNode, std::memory_order_release);

        // Attempt to replace node with newParentNode in the parent
        if (!node->trySetChild(index, node, newParentNode)) {
            delete splitNode;
            delete newParentNode;
            return false;
        }
    }

    // If at end of key, set value
    if (commonPrefixLength + depth == key.length()) {
        node->isTerminal = true;
        node->value = value;
        return true;
    } else {
        int index = key[depth + commonPrefixLength] - 'a';
        return insertRec(node->getChild(index), key, value, depth + commonPrefixLength);
    }
}

template <typename O>
O RadixTreeParallel<O>::getValueForExactKey(const std::string& key) {
    RadixNode<O>* node = root.load(std::memory_order_acquire);
    int depth = 0;

    while (node != nullptr) {
        int commonPrefixLength = 0;
        while (commonPrefixLength < node->key.length() && commonPrefixLength + depth < key.length() &&
                node->key[commonPrefixLength] == key[depth + commonPrefixLength]) {
            ++commonPrefixLength;
        }

        if (commonPrefixLength == node->key.length() && commonPrefixLength + depth == key.length()) {
            return node->isTerminal ? node->value : O();
        }

        depth += commonPrefixLength;
        int index = key[depth] - 'a';
        node = node->getChild(index);
    }

    return O();
}