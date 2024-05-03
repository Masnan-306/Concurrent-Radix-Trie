#include <atomic>
#include <array>
#include <memory>
#include <string>
#include <iostream>

// RadixNode implementation
template <typename O>
RadixNode<O>::RadixNode(const std::string& key, const O& value, bool isTerminal) 
    : key(key), value(value), isTerminal(isTerminal), 
      children(std::make_shared<std::array<std::atomic<RadixNode<O>*>, 26>>()) {
    for (auto& child : *children) {
        child.store(nullptr, std::memory_order_relaxed);
    }
}

template <typename O>
bool RadixNode<O>::trySetChild(int index, RadixNode<O>* expected, RadixNode<O>* newValue) {
    bool res = (*children)[index].compare_exchange_strong(expected, newValue,
                                                      std::memory_order_release,
                                                      std::memory_order_relaxed);
    return res;
}

template <typename O>
RadixNode<O>* RadixNode<O>::getChild(int index) const {
    return (*children)[index].load(std::memory_order_acquire);
}

template <typename O>
bool RadixTreeParallel<O>::insertHelper(RadixNode<O>* root, const std::string& key, const O& value) {
    RadixNode<O>* node = root;
    RadixNode<O>* parent = root;
    int depth = 0;

    while (true) {
        // Compute common prefix length
        int commonPrefixLength = 0;
        while (commonPrefixLength < node->key.length() && commonPrefixLength + depth < key.length() && node->key[commonPrefixLength] == key[depth + commonPrefixLength]) {
            ++commonPrefixLength;
        }

        if (commonPrefixLength < node->key.length()) {
            int index = node->key[commonPrefixLength] - 'a';

            // Node splitting required
            auto newNodeTop = new RadixNode<O>(node->key.substr(0, commonPrefixLength), O(), false);
            auto newNodeBottom  = new RadixNode<O>(node->key.substr(commonPrefixLength), node->value, node->isTerminal);
            newNodeBottom->children = node->children;  // Transfer children

            // Perform atomic update on child
            (*newNodeTop->children)[index].store(newNodeBottom);
            if (commonPrefixLength + depth == key.length()) {
                newNodeTop->isTerminal = true;
                newNodeTop->value = value;
                return parent->trySetChild(node->key[0] - 'a', node, newNodeTop);
            } else if (!parent->trySetChild(node->key[0] - 'a', node, newNodeTop)){
                return false;
            }
            node = newNodeTop;
        }

        if (commonPrefixLength + depth == key.length()) {
            
            auto newNode = new RadixNode<O>(node->key, value, true);
            bool ans = parent->trySetChild(node->key[0] - 'a', node, newNode);
            return ans;
        }

        // Move deeper in the trie
        int index = key[depth + commonPrefixLength] - 'a';
        depth += commonPrefixLength;
        RadixNode<O>* nextNode = node->getChild(index);
        if (!nextNode) {
            RadixNode<O>* newNode = new RadixNode<O>(key.substr(depth), value, true);
            return node->trySetChild(index, nullptr, newNode);
        }
        parent = node;
        node = nextNode;
    }
    return false;
}

template <typename O>
void RadixTreeParallel<O>::put(const std::string& key, const O& value) {
    while (!insertHelper(root, key, value)){};
}

template <typename O>
O RadixTreeParallel<O>::getValueForExactKey(const std::string& key) {
    RadixNode<O>* node = root;
    int depth = 0;

    while (node) {
        if (depth == key.length() && node->isTerminal) {
            return node->value;
        }

        int commonPrefixLength = 0;
        while (commonPrefixLength < node->key.length() && commonPrefixLength + depth < key.length() && node->key[commonPrefixLength] == key[depth + commonPrefixLength]) {
            ++commonPrefixLength;
        }

        if (commonPrefixLength == node->key.length() && commonPrefixLength + depth == key.length()) {
            if (node->isTerminal) {
                return node->value;
            } else {
                break;
            }
        }

        if (commonPrefixLength < node->key.length() || depth + commonPrefixLength >= key.length()) {
            break;
        }

        int index = key[depth + commonPrefixLength] - 'a';
        node = node->getChild(index);
        depth += commonPrefixLength;
    }

    return O(); // Return default-constructed object if the key is not found
}

template <typename O>
void RadixTreeParallel<O>::printTree(RadixNode<O>* node, const std::string& prefix, const std::string& childPrefix) const {
    if (!node) {
        return;
    }

    // Print the current node's key
    std::cout << prefix;
    std::cout << (node->isTerminal ? " (" + node->key + ") -> " + std::to_string(node->value) : node->key) << std::endl;

    // Recursively print each child, passing a new prefix for children
    for (int i = 0; i < 26; ++i) {
        if (node->getChild(i) != nullptr) {
            // Last child or not to determine the branch drawing
            bool isLast = (i == 26 - 1);
            std::cout << childPrefix << (isLast ? "└── " : "├── ");
            printTree(node->getChild(i), "", childPrefix + (isLast ? "    " : "│   "));
        }
    }
}

template <typename O>
void RadixTreeParallel<O>::print() const {
    std::cout << "Radix Trie Structure:" << std::endl;
    printTree(root, "", "");
}
