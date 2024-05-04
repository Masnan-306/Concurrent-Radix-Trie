// RadixTrieLock.tpp
#include "RadixTrieFineLock.h"
#include <iostream>

template <typename O>
void RadixTreeParallel<O>::insertIterative(RadixNode<O>* root, const std::string& key, const O& value) {
    RadixNode<O>* node = root;
    int depth = 0;

    while (true) {

        std::lock_guard<std::mutex> lock(node->nodeMutex);  // Lock each node as we traverse

        // Compute common prefix length
        int commonPrefixLength = 0;
        while (commonPrefixLength < node->key.length() && commonPrefixLength + depth < key.length() && node->key[commonPrefixLength] == key[depth + commonPrefixLength]) {
            ++commonPrefixLength;
        }

        // Split the node if the common prefix length is less than the node's key length
        // Length of root key is 0 so it wouldn't enter this clause
        if (commonPrefixLength < node->key.length()) {
            auto newNode = new RadixNode<O>(node->key.substr(commonPrefixLength), node->value, node->isTerminal);
            newNode->children = node->children;  // New node takes over the original node's children
            
            int index = node->key[commonPrefixLength] - 'a';
            node->key = node->key.substr(0, commonPrefixLength); // Truncate the original node's key
            node->children.fill(nullptr);
            node->children[index] = newNode;  // Point the original node to the new node
            node->isTerminal = false;
            node->value = O();  // Reset the value as it is no longer a terminal node
        }

        if (commonPrefixLength + depth == key.length()) {
            node->isTerminal = true;
            node->value = value;
            break;
        } else {
            int index = key[depth + commonPrefixLength] - 'a';
            depth += commonPrefixLength;
            RadixNode<O>* nextNode = node->children[index];

            // Encounter Leaf node and we can insert all the suffix
            if (!nextNode) {
                node->children[index] = new RadixNode<O>(key.substr(depth), value, true);
                return;
            }
            node = nextNode; // Move to the child node
        }
    }

    return;
}



template <typename O>
void RadixTreeParallel<O>::put(const std::string& key, const O& value) {
    insertIterative(root, key, value);
}

template <typename O>
O RadixTreeParallel<O>::getValueForExactKey(const std::string& key) {
    RadixNode<O>* node = root;
    int depth = 0;

    while (node) {
        std::lock_guard<std::mutex> lock(node->nodeMutex); // Lock each node as we traverse

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
        node = node->children[index];
        depth += commonPrefixLength;
    }

    return O(); // Return default-constructed object if the key is not found
}


template <typename O>
std::vector<std::pair<std::string, O>> RadixTreeParallel<O>::collectPairs(const std::string& prefix) const {
    std::vector<std::pair<std::string, O>> pairs;

    RadixNode<O>* node = root;
    int depth = 0;

    // Traverse down the trie to the node corresponding to the prefix
    while (node) {
        std::lock_guard<std::mutex> lock(node->nodeMutex);

        depth += node->key.length();

        if (depth > prefix.length()) {
            return pairs;
        }
        
        if (depth == prefix.length()) {
            break;
        }

        int index = prefix[depth] - 'a';
        node = node->children[index];
    }

    // If the prefix doesn't exist in the trie, return empty pairs
    if (!node) {
        return pairs;
    }

    // Perform depth-first traversal to collect pairs
    collectPairsDFS(node, prefix, pairs);

    return pairs;
}

template <typename O>
void RadixTreeParallel<O>::collectPairsDFS(RadixNode<O>* node, const std::string& prefix, std::vector<std::pair<std::string, O>>& pairs) const {
    std::stack<std::pair<RadixNode<O>*, std::string>> nodeStack;
    nodeStack.push({node, prefix});

    while (!nodeStack.empty()) {
        auto [currentNode, currentPrefix] = nodeStack.top();
        nodeStack.pop();

        std::lock_guard<std::mutex> lock(currentNode->nodeMutex);

        // If the current node is a terminal node, add its key-value pair to the result
        if (currentNode->isTerminal) {
            pairs.emplace_back(currentPrefix + currentNode->key, currentNode->value);
        }

        // Traverse all child nodes and push them onto the stack
        for (int i = 0; i < 26; i++) {
            if (currentNode->children[i]) {
                std::string childPrefix = currentPrefix + currentNode->key;
                nodeStack.push({currentNode->children[i], childPrefix});
            }
        }
    }
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
    for (int i = 0; i < node->children.size(); ++i) {
        if (node->children[i] != nullptr) {
            // Last child or not to determine the branch drawing
            bool isLast = (i == node->children.size() - 1);
            std::cout << childPrefix << (isLast ? "└── " : "├── ");
            printTree(node->children[i], "", childPrefix + (isLast ? "    " : "│   "));
        }
    }
}

template <typename O>
void RadixTreeParallel<O>::print() const {
    std::cout << "Radix Trie Structure:" << std::endl;
    printTree(root, "", "");
}
