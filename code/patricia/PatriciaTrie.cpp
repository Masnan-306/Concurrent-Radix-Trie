#include "PatriciaTree.h"

std::string stringToBinary(const std::string& input) {
    std::string binaryString;
    for (char ch : input) {
        // Convert character's ASCII value to binary representation
        std::bitset<8> binary(ch);
        // Append binary representation to the result string
        binaryString += binary.to_string();
    }
    return binaryString;
}

std::shared_ptr<PatriciaNode> PatriciaTree::createNode(const std::string& key, const std::string& value, int diff) {
    return std::make_shared<PatriciaNode>(key, value, diff);
}

PatriciaTree::PatriciaTree() : root(nullptr) {}

int PatriciaTree::computeDiff(const std::shared_ptr<PatriciaNode>& node1, const std::string& key) {
    const std::string& nodeKey = (node1) ? node1->key : "00000000";
    int minSize = std::min(nodeKey.size(), key.size());
    
    // Find the position where the concatenated binary representations differ
    int diff = 0;
    while (diff < minSize && nodeKey[diff] == key[diff]) {
        diff++;
    }
    std::cout << nodeKey << " " << key << " " << diff << std::endl;
    return diff;
}

int PatriciaTree::bitAtPosition(const std::string& key, int position) {
    // Ensure position is within bounds of the concatenated binary string
    if (position >= 0 && position < key.size()) {
        // Extract the bit at the specified position
        return key[position] - '0'; // Convert char to int (0 or 1)
    }
    // Return -1 to indicate error or out of bounds
    return -1;
}

std::shared_ptr<PatriciaNode> PatriciaTree::searchHelper(const std::string& key) {
    if (root == nullptr || root->key == key) {
        return root;
    }
    auto parent = root;
    std::cout << "Root: " << root->value  << std::endl;
    auto child = root->left;

    while (child != nullptr && parent->diff < child->diff) {
        int bit = bitAtPosition(key, child->diff);
        
        parent = child;
        child = (bit == 0) ? child->left : child->right;
    }
    return child;
}

bool PatriciaTree::insert(const std::string& s) {
    const std::string key = stringToBinary(s);
    return insertHelper(key, s);
}

bool PatriciaTree::insertHelper(const std::string& key, const std::string& value) {
    if (root == nullptr) {
        int diff = computeDiff(nullptr, key);
        root = createNode(key, value, diff);
        root->left = root;
        return true;
    }

    auto found = searchHelper(key);
    if (found != nullptr && found->key == key) {
        found->value = value;
        return true;
    }

    auto parent = root;
    auto child = root->left;
    int diff = computeDiff(child, key);

    while (child != nullptr && parent->diff < child->diff && diff > child->diff) {
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

    bit = bitAtPosition(parent->key, diff);
    if (bit == 1 && node == root){
        parent->left = node;
    } else{
        parent->right = node;
    }
    return true;
}

bool PatriciaTree::search(const std::string& key) {
    auto node = searchHelper(key);
    return node != nullptr && node->key == key;
}

void PatriciaTree::printTrie() {
    printNode(root, "");
}

void PatriciaTree::printNode(const std::shared_ptr<PatriciaNode>& node, const std::string& indent) {
    if (!node) {
        return;
    }
    // Output the node data
    std::cout << indent << "Key: " << node->key << " | Value: " << node->value << " | Diff: " << node->diff << std::endl;
    // Recurse on left child
    if (node->left != nullptr && node->left != node) {
        std::cout << indent << "L-> " << std::endl;
        printNode(node->left, indent + "   ");
    }
    // Recurse on right child
    if (node->right != nullptr && node->right != node) {
        std::cout << indent << "R-> " << std::endl;
        printNode(node->right, indent + "   ");
    }
}
