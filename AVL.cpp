#ifndef __PROGTEST__

#include <cassert>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>

#endif
template <typename Product>
struct Bestsellers {
    //Node structure in AVL tree
    struct Node {
        //Pointers to parents and children in AVL tree
        Node * left = nullptr;
        Node * right = nullptr;
        Node * parent = nullptr;

        //height of the node, used in calculating tree balance
        int height = 1;

        //size of subtree starting with node
        size_t subtreeSize = 1;
        size_t subtreeSum = 0;

        //key of node
        size_t sales = 0;

        //vector of products with the same amount of sold items
        std::vector<Product> products;

        void updateHeight() {
            if (left && right) {
                height = std::max(left->height, right->height) + 1;
            }
            else if (left) {
                height = left->height + 1;
            }
            else if (right) {
                height = right->height + 1;
            }
            else {
                height = 1;
            }
        }

        long long int getBalance() {
            if (left && right) {
                return right->height - left->height;
            }
            else if (left) {
                return left->height * -1;
            }
            else if (right) {
                return right->height;
            }
            else {
                return 0;
            }
        }

        //check if current node is left child of parent
        bool isLeftChild() {
            if (parent == nullptr) {
                throw std::runtime_error("Something went terribly wrong. isLeftChild is checking a nullptr parent");
            }
            if (parent->left == this) {
                return true;
            }
            else {
                return false;
            }
        }

        void copyData(Node * src) {
            products = src->products;
            sales = src->sales;
        }

        void updateSum() {
            size_t retVal = products.size() * sales;
            if (left != nullptr) {
                retVal += left->subtreeSum;
            }
            if (right != nullptr) {
                retVal += right->subtreeSum;
            }

            subtreeSum = retVal;
        }

        void updateSize() {
            size_t retVal = products.size();
            if (left != nullptr) {
                retVal += left->subtreeSize;
            }
            if (right != nullptr) {
                retVal += right->subtreeSize;
            }

            subtreeSize = retVal;
        }
    };

    //map of products and the node they are located in
    std::unordered_map<Product, Node *> map;

    //pointer to root of AVL tree
    Node * root = nullptr;

    /// ==================== Helper functions ====================

    Node * leftRotate(Node * node) {
        Node * y = node->right;
        Node * x = node;

        //swap nodes node and y
        y->parent = x->parent;
        if (y->parent) {
            if (x->isLeftChild()) {
                y->parent->left = y;
            }
            else {
                y->parent->right = y;
            }
        }

        x->right = y->left;
        if (x->right) {
            x->right->parent = x;
        }

        y->left = x;
        x->parent = y;

        x->updateHeight();
        y->updateHeight();

        x->updateSize();
        x->updateSum();

        y->updateSize();
        y->updateSum();

        if (node == root) {
            root = y;
        }

        return y;
    }

    Node * rightRotate(Node * node) {
        Node * x = node->left;
        Node * y = node;

        x->parent = y->parent;
        if (x->parent) {
            if (y->isLeftChild()) {
                x->parent->left = x;
            }
            else {
                x->parent->right = x;
            }
        }

        y->left = x->right;
        if (y->left) {
            y->left->parent = y;
        }

        x->right = y;
        y->parent = x;

        y->updateHeight();
        x->updateHeight();

        y->updateSize();
        y->updateSum();

        x->updateSize();
        x->updateSum();

        if (node == root) {
            root = x;
        }

        return x;
    }

    //Function to find out depth of subtree
    size_t maxDepth(Node * node) {
        if (node == nullptr)
            return 0;
        else {
            size_t lDepth = maxDepth(node->left);
            size_t rDepth = maxDepth(node->right);

            if (lDepth > rDepth)
                return (lDepth + 1);
            else
                return (rDepth + 1);
        }
    }

    Node * minValue(Node * node) {
        Node * current = node;

        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    Node * inOrderSuccessor(Node * node) {
        if (node->right != nullptr) {
            return minValue(node->right);
        }

        Node * parent = node->parent;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

    Node * balance(Node * node) {
        if (node->getBalance() == 2) {
            if (node->right->getBalance() != -1) {
                return leftRotate(node);
            }
            else {
                //RL rotation
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }
        else if (node->getBalance() == -2) {
            if (node->left->getBalance() != 1) {
                return rightRotate(node);
            }
            else {
                //LR rotation
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }
        return nullptr;
    }

    void checkBalance(Node * node) {
        if (node == nullptr) {
            return;
        }

        node->updateHeight();

        if (node->getBalance() < -1 || node->getBalance() > 1) {
            node = balance(node);
        }

        if (node == nullptr) {
            return;
        }
        checkBalance(node->parent);
    }

    void propagate(Node * node) {
        if (node == nullptr) {
            return;
        }

        node->updateSum();
        node->updateSize();

        propagate(node->parent);
    }

    //finds the largest rank of node using a key, returns a pair with node pointer and largest rank of the node.
    std::pair<Node *, size_t> find(Node * node, size_t key, size_t rank) const {
        if (node == nullptr) {
            return std::make_pair(nullptr, 0);
        }

        if (key < node->sales) {
            return find(node->left, key, rank);
        }
        else if (key > node->sales) {
            if (node->left) {
                rank -= node->left->subtreeSize;
            }
            return find(node->right, key, rank - node->products.size());
        }
        else {
            if (node->left != nullptr) {
                rank -= node->left->subtreeSize;
            }
            return std::make_pair(node, rank);
        }
    }

    size_t getNodeMaxRank(Node * node) const {
        size_t retVal = node->products.size();
        if (node->right != nullptr) {
            retVal += node->right->subtreeSize;
        }

        return retVal;
    }

    size_t getNodeMinRank(Node * node) const {
        size_t retVal = 1;
        if (node->right != nullptr) {
            retVal += node->right->subtreeSize;
        }

        return retVal;
    }

    Product & find(Node * node, size_t rank) const {
        if (rank < getNodeMinRank(node)) {
            return find(node->right, rank);
        }
        else if (rank > getNodeMaxRank(node)) {
            return find(node->left, rank - getNodeMaxRank(node));
        }
        else {
            return node->products[rank - getNodeMinRank(node)];
        }
    }

    void delNode(Node * node) {
        if (node->left != nullptr && node->right != nullptr) {
            Node * successor = inOrderSuccessor(node);
            node->copyData(successor);

            for (auto & i : node->products) {
                map[i] = node;
            }

            delNode(successor);
        }
        else if (node->left != nullptr) {
            node->left->parent = node->parent;

            if (node->parent != nullptr) {
                if (node->isLeftChild()) {
                    node->parent->left = node->left;
                }
                else {
                    node->parent->right = node->left;
                }
            }
            else {
                root = node->left;
            }

            propagate(node->left);
            checkBalance(node->left);
            delete node;
        }
        else if (node->right != nullptr) {
            node->right->parent = node->parent;

            if (node->parent != nullptr) {
                if (node->isLeftChild()) {
                    node->parent->left = node->right;
                }
                else {
                    node->parent->right = node->right;
                }
            }
            else {
                root = node->right;
            }

            propagate(node->right);
            checkBalance(node->right);
            delete node;
        }
        else {
            if (node == root) {
                root = nullptr;
                delete node;

                return;
            }
            else {
                if (node->isLeftChild()) {
                    node->parent->left = nullptr;
                }
                else {
                    node->parent->right = nullptr;
                }

                propagate(node->parent);
                checkBalance(node->parent);

                delete node;
            }
        }
    }

    void delProd(Node * node, const Product & p) {
        auto it = std::find(node->products.begin(), node->products.end(), p);

        if (it == node->products.end()) {
            throw std::runtime_error("This shouldnt happen. If a product is in map with a node, it SHOULD be in here");
        }

        node->products.erase(it);

        if (node->products.empty()) {
            delNode(node);
        }
        else {
            propagate(node);
        }
    }

    void insert(Node * node, Node * prev, const Product & p, size_t key) {
        if (node == nullptr) {
            Node * tmp = new Node;

            tmp->sales = key;
            tmp->products.push_back(p);
            map[p] = tmp;
            tmp->parent = prev;

            if (prev != nullptr) {
                if (key < prev->sales) {
                    prev->left = tmp;
                }
                else {
                    prev->right = tmp;
                }
            }

            if (root == nullptr) {
                root = tmp;
            }

            checkBalance(tmp);
            propagate(tmp);
            return;
        }

        if (key < node->sales) {
            insert(node->left, node, p, key);
        }
        else if (key > node->sales) {
            insert(node->right, node, p, key);
        }
        else {
            node->products.push_back(p);
            map[p] = node;
            propagate(node);
        }
    }

    size_t getIndex(Node * node, const Product & p) const {
        for (size_t i = 0; i < node->products.size(); i++) {
            if (node->products[i] == p) {
                return i;
            }
        }
        throw std::runtime_error("This shouldnt happen. Product must be present in vector when calling getIndex");
    }

    size_t removeLowerRange(Node * node, size_t key, size_t total) const {
        if (key < node->sales) {
            return removeLowerRange(node->left, key, total - (node->subtreeSum - node->left->subtreeSum));
        }
        else if (key > node->sales) {
            return removeLowerRange(node->right, key, total);
        }
        else {
            if (node->right) {
                return total - node->right->subtreeSum;
            }
            else {
                return total;
            }
        }
    }

    size_t removeUpperRange(Node * node, size_t key, size_t total) const {
        if (key < node->sales) {
            return removeUpperRange(node->left, key, total);
        }
        else if (key > node->sales) {
            return removeUpperRange(node->right, key, total - (node->subtreeSum - node->right->subtreeSum));
        }
        else {
            if (node->left) {
                return total - node->left->subtreeSum;
            }
            return total;
        }
    }

    //used by destructor
    void delTree(Node * node) {
        if (node == nullptr) {
            return;
        }

        delTree(node->left);
        delTree(node->right);
        delete node;
    }

    /// ==================== End helper functions ====================

    //destructor
    ~Bestsellers() {
        delTree(root);
    }


    // The total number of tracked products
    size_t products() const { return map.size(); }

    void sell(const Product & p, size_t amount) {
        auto mapFind = map.find(p);
        if (mapFind == map.end()) {
            insert(root, nullptr, p, amount);
        }
        else {
            size_t oldSales = mapFind->second->sales;
            delProd(mapFind->second, p);

            insert(root, nullptr, p, oldSales + amount);
        }
    }

    // The most sold product has rank 1
    size_t rank(const Product & p) const {
        auto mapFind = map.find(p);

        if (mapFind == map.end()) {
            throw std::out_of_range("rank(): Invalid product name");
        }

        //result is max rank of node
        auto result = find(root, mapFind->second->sales, root->subtreeSize);

        return result.second - (mapFind->second->products.size() - getIndex(mapFind->second, p)) + 1;
    }

    const Product & product(size_t rank) const {
        if (rank < 1 || rank > products()) {
            throw std::out_of_range("product(): Invalid rank");
        }
        return find(root, rank);
    }

    // How many copies of product with given rank were sold
    size_t sold(size_t rank) const {
        if (rank < 1 || rank > products()) {
            throw std::out_of_range("sold(): Invalid rank");
        }

        auto mapFind = map.find(find(root, rank));

        return mapFind->second->sales;
    }

    // The same but sum over interval of products (including from and to)
    // It must hold: sold(x) == sold(x, x)
    size_t sold(size_t from, size_t to) const {
        if (from < 1 || from > products() || to < 1 || to > products() || from > to) {
            throw std::out_of_range("sold(): Invalid ranks given");
        }

        //product located at lower threshold position
        Product & lowerP = find(root, from);
        Node * lowerNode = map.find(lowerP)->second;

        //product located at upper threshold position
        Product & upperP = find(root, to);
        Node * upperNode = map.find(upperP)->second;

        size_t total = removeLowerRange(root, lowerNode->sales, root->subtreeSum);
        total = removeUpperRange(root, upperNode->sales, total);

        size_t lowerIndex = getIndex(lowerNode, lowerP);
        size_t upperIndex = getIndex(upperNode, upperP);

        total -= lowerNode->sales * lowerIndex;
        total -= upperNode->sales * (upperNode->products.size() - 1 - upperIndex);
        return total;
    }

    // Bonus only, ignore if you are not interested in bonus
    // The smallest (resp. largest) rank with sold(rank) == sold(r)
    size_t first_same(size_t r) const {
        Product p = product(r);
        auto nodeAndRank = find(root, map.find(p)->second->sales, root->subtreeSize);

        return nodeAndRank.second - nodeAndRank.first->products.size() + 1;
    }

    size_t last_same(size_t r) const {
        Product p = product(r);
        auto nodeAndRank = find(root, map.find(p)->second->sales, root->subtreeSize);

        return nodeAndRank.second;
    }
};

#ifndef __PROGTEST__

void test1() {
    Bestsellers<std::string> T;
    T.sell("coke", 32);
    T.sell("bread", 1);
    assert(T.products() == 2);
    T.sell("ham", 2);
    T.sell("mushrooms", 12);

    assert(T.products() == 4);
    assert(T.rank("ham") == 3);
    assert(T.rank("coke") == 1);
    assert(T.sold(1, 3) == 46);
    assert(T.product(2) == "mushrooms");

    T.sell("ham", 11);
    assert(T.products() == 4);
    assert(T.product(2) == "ham");
    assert(T.sold(2) == 13);
    assert(T.sold(2, 2) == 13);
    assert(T.sold(1, 2) == 45);
}

void test2() {
# define CATCH(expr) \
  try { expr; assert(0); } catch (const std::out_of_range&) { assert(1); };

    Bestsellers<std::string> T;
    T.sell("coke", 32);
    T.sell("bread", 1);

    CATCH(T.rank("ham"));
    CATCH(T.product(3));
    CATCH(T.sold(0));
    CATCH(T.sold(9));
    CATCH(T.sold(0, 1));
    CATCH(T.sold(3, 2));
    CATCH(T.sold(1, 9));

#undef CATCH
}

int main() {
    test1();
    test2();

    return 0;
}

#endif


