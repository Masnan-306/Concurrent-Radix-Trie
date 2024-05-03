#include <iostream>
#include <string>
#include <array>
#include <memory>

template<typename T>
class Node {
private:
    std::string prefix;
    T value;
    std::array<std::atomic<Node<T>*>, 256> edges;
    std::atomic<int> children;

public:
    Node() : children(0) {
        // Initialize all edges to nullptr in the constructor instead
        for (auto& edge : edges) {
            edge.store(nullptr, std::memory_order_relaxed);
        }
    }

    Node<T>* next(unsigned char b) const {
        return edges[b].load(std::memory_order_acquire);
    }

    void setNext(unsigned char b, Node<T>* node) {
        edges[b].store(node, std::memory_order_release);
        children.fetch_add(1, std::memory_order_relaxed);
    }

    bool swapNext(unsigned char b, Node<T>* existing, Node<T>* next) {
        Node<T>* expected = existing;
        if (edges[b].compare_exchange_strong(expected, next, std::memory_order_acq_rel)) {
            children.fetch_add(1, std::memory_order_relaxed);
            return true;
        }
        return false;
    }

    // Ensure all edges are initialized and ready to use
    void setupEdges() {
        for (auto& edge : edges) {
            if (edge.load(std::memory_order_acquire) == nullptr) {
                edge.store(new Node<T>(), std::memory_order_release);  // This line initializes an edge if it's nullptr
            }
        }
    }

    bool leaf() const {
        return children.load(std::memory_order_acquire) < 1;
    }

    void print() const {
        std::cout << "{\n";
        std::cout << "    Prefix Length: " << prefix.length() << "\n";
        std::cout << "    Prefix: " << prefix << "\n";
        std::cout << "    Value: "; // Assuming value has an overload for << operator
        std::cout << value << "\n";
        std::cout << "    Edges: [";
        bool first = true;
        for (int i = 0; i < 256; ++i) {
            Node<T>* edge = next(i);
            if (edge) {
                if (!first) std::cout << ", ";
                std::cout << char(i) << ": " << edge->prefix;
                first = false;
            }
        }
        std::cout << "]\n";
        std::cout << "}\n";
    }
};

template <typename O>
bool shouldInsert(const std::string& key, const std::shared_ptr<RadixNode<O>>& node, const std::shared_ptr<RadixNode<O>>& parent, int pos, int dv) {
    return pos < key.length() && node == nullptr;
}

template <typename O>
bool shouldUpdate(const std::string& key, const std::shared_ptr<RadixNode<O>>& node, const std::shared_ptr<RadixNode<O>>& parent, int pos, int dv) {
    return (pos == key.length() && dv == node->key.length()) || (pos == key.length() && node->key.empty());
}

template <typename O>
bool shouldSplitTwoWay(const std::string& key, const std::shared_ptr<RadixNode<O>>& node, const std::shared_ptr<RadixNode<O>>& parent, int pos, int dv) {
    return (key.length() - (pos + dv)) == 0;
}

template <typename O>
bool shouldSplitThreeWay(const std::string& key, const std::shared_ptr<RadixNode<O>>& node, const std::shared_ptr<RadixNode<O>>& parent, int pos, int dv) {
    return (key.length() - (pos + dv)) > 0;
}

int divergence(const std::string& prefix, const std::string& key) {
    int i = 0;
    int minLength = std::min(key.length(), prefix.length());

    while (i < minLength) {
        if (key[i] != prefix[i]) {
            break;
        }
        i++;
    }

    return i;
}

template <typename O>
class Radix {

private:
    std::shared_ptr<RadixNode<O>> root;
    
    bool insertNode(const std::string& key, const T& value, Node<T>* parent, Node<T>* node, int pos, int dv) {
        if (pos + 1 < key.size()) {
            std::string newKey = key.substr(pos + 1);
            Node<T>* newNode = new Node<T>(newKey, value);
            return parent->swapNext(key[pos], nullptr, newNode);
        }
        return false;
    }

    bool updateNode(const std::string& key, const T& value, Node<T>* parent, Node<T>* node, int pos, int dv) {
        if (node) {
            node->value = value;
            return true;
        }
        return false;
    }

    bool splitTwoWay(const std::string& key, const T& value, Node<T>* parent, Node<T>* node, int pos, int dv) {
        if (key.size() > pos) {
            std::string newKey = key.substr(pos, dv);
            Node<T>* newNode = new Node<T>(newKey, value);

            if (dv < node->prefix.size()) {
                std::string remainingKey = node->prefix.substr(dv);
                Node<T>* remainingNode = new Node<T>(remainingKey, node->value);
                newNode->setNext(remainingKey[0], remainingNode);
            }

            return parent->swapNext(key[pos-1], node, newNode);
        }
        return false;
    }

    bool splitThreeWay(const std::string& key, const T& value, Node<T>* parent, Node<T>* node, int pos, int dv) {
        Node<T>* splitNode = new Node<T>(node->prefix.substr(0, dv));
        Node<T>* suffixNode = new Node<T>(node->prefix.substr(dv + 1), node->value);
        Node<T>* newNode = new Node<T>(key.substr(pos + dv + 1), value);

        splitNode->setNext(node->prefix[dv], suffixNode);
        splitNode->setNext(key[pos + dv], newNode);

        return parent->swapNext(key[pos-1], node, splitNode);
    }

    std::tuple<Node<T>*, Node<T>*, int, int> find(const std::string& key) {
        Node<T>* node = root, *parent = nullptr;
        int pos = 0, dv = 0;

        while (node && pos < key.size()) {
            parent = node;
            node = node->next(key[pos]);
            if (!node) break;

            dv = divergence(node->prefix, key.substr(pos));
            if (node->prefix.size() > dv) {
                return {parent, node, pos, dv};
            }
            pos += dv;

            if (pos == key.size()) {
                return {parent, node, pos, dv};
            }
        }

        return {node, nullptr, pos, dv};
    }


public:
    Radix() : root(std::make_shared<RadixNode<O>>("")) {}

     bool Insert(const std::string& key, const O& value) {
        bool success = false;
        auto [parent, node, pos, dv] = find(key);

        while (true) {
            if (shouldInsert(key, node, parent, pos, dv)) {
                success = insertNode(key, value, parent, node, pos, dv);
            } else if (shouldUpdate(key, node, parent, pos, dv)) {
                success = updateNode(key, value, parent, node, pos, dv);
            } else if (shouldSplitThreeWay(key, node, parent, pos, dv)) {
                success = splitThreeWay(key, value, parent, node, pos, dv);
            } else if (shouldSplitTwoWay(key, node, parent, pos, dv)) {
                success = splitTwoWay(key, value, parent, node, pos, dv);
            }

            if (success) {
                return true;
            }

            std::tie(parent, node, pos, dv) = find(key);
        }
    }

    std::shared_ptr<O> Lookup(const std::string& key) {
        auto [parent, node, pos, dv] = find(key);

        if (node && pos == key.size() && node->isTerminal) {
            return std::make_shared<O>(node->value);
        }
        return nullptr;
    }

};

int main() {
    // Create an instance of Radix using string as the template parameter
    Radix<int> myRadix;

    // Insert some keys with their corresponding values
    myRadix.Insert("apple", 1);
    myRadix.Insert("app", 2);
    myRadix.Insert("ape", 3);
    myRadix.Insert("banana", 4);
    myRadix.Insert("band", 5);
    myRadix.Insert("bandana", 6);

    // Try to retrieve and print the values
    auto printValue = [&](const std::string& key) {
        auto value = myRadix.Lookup(key);
        if (value) {
            std::cout << "Key: " << key << " -> Value: " << *value << std::endl;
        } else {
            std::cout << "Key: " << key << " -> Value not found." << std::endl;
        }
    };

    // Keys to lookup
    printValue("apple");
    printValue("app");
    printValue("ape");
    printValue("banana");
    printValue("band");
    printValue("bandana");
    printValue("bana"); // This should not find a value

    return 0;
}
