// RadixTrieLock.tpp
#include "RadixTrieFineLock.h"
#include <iostream>
#include <vector>  // Required for std::vector
#include <stack>   // Required for std::stack

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

        if (commonPrefixLength < node->key.length()) {
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

template <typename O>
void testAndRemoveNode(RadixNode<O>* node, RadixNode<O>* parent) {
    RadixNode<O>* child;
    int count = 0;
    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            child = node->children[i];
            count++;
        }
    }
    
    // Remove node if its number of children <= 1
    // Coalesce node's key with child's key if there is a child
    if (count == 1) {
        child->nodeMutex.lock();
        child->key = node->key + child->key;
        parent->children[node->key[0] - 'a'] = child;
        child->nodeMutex.unlock();
    } else if (count == 0) {
        parent->children[node->key[0] - 'a'] = nullptr;
    }
}

template <typename O>
void RadixTreeParallel<O>::testAndCoalesceChild(RadixNode<O>* node, RadixNode<O>* parent) {
    // Count parent's number of children
    RadixNode<O>* child;
    int count = 0;
    for (int i = 0; i < 26; i++) {
        if (parent->children[i]) {
            child = parent->children[i];
            count++;
        }
    }
    
    // We don't need to coalesce because there is more than one child or
    // We can not coalesce parent with its child because parent is terminated
    if (count != 1 || parent->isTerminal || parent == root) {
        return;
    }

    // node under parent could be removed. If so, lock is needed
    if (child != node) {
        child->nodeMutex.lock();
    }

    // Coalesce child with parent if there is only one child
    parent->key += child->key;
    parent->children = child->children;
    parent->value = child->value;
    parent->isTerminal = child->isTerminal;

    if (child != node) {
        child->nodeMutex.unlock();
    }
}

template <typename O>
void RadixTreeParallel<O>::removeKey(const std::string& key) {
    // std::cout << "Deleting " << key << std::endl;
    RadixNode<O>* node = root;
    RadixNode<O>* parent = new RadixNode<O>("");
    parent->nodeMutex.lock();
    int depth = 0;

    // Look for the terminated node
    while (node) {
        node->nodeMutex.lock();

        int commonPrefixLength = 0;
        while (commonPrefixLength < node->key.length() && commonPrefixLength + depth < key.length() && node->key[commonPrefixLength] == key[depth + commonPrefixLength]) {
            ++commonPrefixLength;
        }

        // Found the terminated node
        if (commonPrefixLength + depth == key.length()) {
            break;
        }

        // Halfway in a node, so key not exist
        if (commonPrefixLength < node->key.length()) {
            std::cout << "Key " << key << " not found" << std::endl;
            node->nodeMutex.unlock();
            parent->nodeMutex.unlock();
            return;
        }         

        // Goes deeper into the trie since there must be trailig key
        int index = key[depth + commonPrefixLength] - 'a';
        parent->nodeMutex.unlock();
        parent = node;
        node = node->children[index];
        depth += commonPrefixLength;
    }
    
    // Delete node and compress if necessary
    if (node && node->isTerminal) {
        node->isTerminal = false;
        node->value = O();
        testAndRemoveNode(node, parent);
        testAndCoalesceChild(node, parent);
    }

    if (node) {
        node->nodeMutex.unlock();
    }
    parent->nodeMutex.unlock();
}
