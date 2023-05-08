//
// Created by chenzhi on 2023/5/8.
//


#include <iostream>

//AVL树是一种自平衡的二叉搜索树，它在每个节点上维护一个平衡因子（Balance Factor），用来衡量左子树和右子树的高度差。AVL树保持以下性质：
//
//对于任意节点，其左子树和右子树的高度差不超过1（即平衡因子的绝对值不超过1）。
//每个节点的左子树和右子树都是AVL树。
//AVL树的平衡性保证了在最坏情况下，对于搜索、插入和删除操作的时间复杂度都是O(log n)，其中n是树中节点的数量。
//
//AVL树的插入和删除操作需要对节点进行旋转来保持平衡。旋转操作包括四种情况：左旋转、右旋转、左右旋转和右左旋转。
//
//左旋转：对于一个节点的右子树较高，进行左旋转可以将其变为左子树较高的情况。
//右旋转：对于一个节点的左子树较高，进行右旋转可以将其变为右子树较高的情况。
//左右旋转：对于一个节点的左子树的右子树较高，进行左右旋转可以将其变为左子树较高的情况。
//右左旋转：对于一个节点的右子树的左子树较高，进行右左旋转可以将其变为右子树较高的情况。
//通过这些旋转操作和平衡因子的维护，AVL树可以在插入和删除节点时自动调整节点的位置，保持树的平衡性。
//
//与红黑树相比，AVL树的平衡性要更严格，因此在频繁的插入和删除操作中，AVL树可能需要更多的平衡调整操作。然而，在需要频繁的搜索操作且对平衡性要求更高的场景下，AVL树是一个较好的选择。
//
//AVL树也广泛应用于各种数据结构和算法中，如数据库索引、图形学中的空间分区等。

struct Node {
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    Node* root;

    int getHeight(Node* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int getBalanceFactor(Node* node) {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    void updateHeight(Node* node) {
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        node->height = std::max(leftHeight, rightHeight) + 1;
    }

    Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    Node* rebalance(Node* node) {
        updateHeight(node);
        int balanceFactor = getBalanceFactor(node);

        // Left-Left case
        if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0)
            return rotateRight(node);

        // Right-Right case
        if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0)
            return rotateLeft(node);

        // Left-Right case
        if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right-Left case
        if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* insertNode(Node* node, int key) {
        if (node == nullptr)
            return new Node(key);

        if (key < node->key)
            node->left = insertNode(node->left, key);
        else if (key > node->key)
            node->right = insertNode(node->right, key);
        else
            return node;  // Duplicate keys are not allowed

        return rebalance(node);
    }

    void inOrderTraversal(Node* node) {
        if (node == nullptr)
            return;

        inOrderTraversal(node->left);
        std::cout << node->key << " ";
        inOrderTraversal(node->right);
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(int key) {
        root = insertNode(root, key);
    }

    void printInOrder() {
        inOrderTraversal(root);
        std::cout << std::endl;
    }
};

int main() {
    AVLTree avl;
    avl.insert(10);
    avl.insert(20);
    avl.insert(30);
    avl.insert(40);
    avl.insert(50);
    avl.insert(25);

    std::cout << "In-order traversal of AVL tree: ";
    return 0;
}