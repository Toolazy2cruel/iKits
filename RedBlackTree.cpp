//
// Created by chenzhi on 2023/5/8.
//

#include <iostream>



//红黑树是一种自平衡的二叉搜索树，它在每个节点上增加了一个表示节点颜色的额外属性，可以保持树的平衡性并且具有较好的查找、插入和删除性能。
//
//红黑树的性质如下：
//
//每个节点是红色或黑色。
//根节点是黑色。
//每个叶子节点（NIL 节点，即空节点）是黑色。
//如果一个节点是红色，则它的两个子节点都是黑色。
//对于每个节点，从该节点到其所有后代叶子节点的简单路径上，均包含相同数量的黑色节点（即具有相同的黑色高度）。
//红黑树通过这些性质来保持平衡。在插入和删除节点时，会通过调整节点的颜色和旋转节点的位置来保持这些性质。具体来说，红黑树的插入和删除操作包括以下几个步骤：
//
//插入节点：首先按照二叉搜索树的插入方式将节点插入到适当的位置，并将其颜色设置为红色。
//修正红黑树性质：如果插入导致某些性质被破坏，需要进行修正。修正包括以下情况：
//如果插入的节点是根节点，将其颜色设置为黑色。
//如果插入的节点的父节点是黑色，不需要进行额外操作。
//如果插入的节点的父节点是红色，需要进行进一步的调整来保持性质。
//如果插入的节点的叔叔节点是红色，可以通过改变父节点、叔叔节点和祖父节点的颜色来解决。
//如果插入的节点的叔叔节点是黑色或者不存在，需要通过旋转操作来解决。
//如果插入的节点是其父节点的右子节点且父节点是祖父节点的左子节点，通过左旋转将问题转化为其他情况。
//如果插入的节点是其父节点的左子节点且父节点是祖父节点的左子节点，通过右旋转将问题转化为其他情况。
//如果插入的节点是其父节点的左子节点且父节点是祖父节点的右子节点，通过右旋转和左旋转来解决。
//如果插入的节点是其父节点的右子节点且父节点是祖父节点的右子节点，通过左旋转和右旋转来解决。
//删除节点：删除节点时，首先按照二叉搜索树的删除方式进行删除，然后通过调整颜色和旋转节点来保持红黑树的性质。
//
//红黑树的旋转操作有两种：左旋转和右旋转。
//
//左旋转：以某个节点为支点进行左旋转，将其右子节点变为新的支点，原来的支点成为新支点的左子节点，新支点的左子节点成为原来支点的右子节点。
//右旋转：以某个节点为支点进行右旋转，将其左子节点变为新的支点，原来的支点成为新支点的右子节点，新支点的右子节点成为原来支点的左子节点。
//通过这些旋转操作，可以重新调整节点的位置，以保持红黑树的平衡性。
//
//红黑树的平衡性保证了在最坏情况下，对于搜索、插入和删除操作的时间复杂度都是 O(log n)，其中 n 是树中节点的数量。
//
//红黑树广泛应用于各种数据结构和算法中，如 C++ STL 中的 set 和 map 容器，以及许多数据库和文件系统的实现中，用于高效地维护有序数据集合。

enum class Color { RED, BLACK };

struct Node {
    int key;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    explicit Node(int key) : key(key), color(Color::RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node* root;

    void rotateLeft(Node* node) {
        Node* rightChild = node->right;
        node->right = rightChild->left;
        if (rightChild->left != nullptr)
            rightChild->left->parent = node;
        rightChild->parent = node->parent;
        if (node->parent == nullptr)
            root = rightChild;
        else if (node == node->parent->left)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;
        rightChild->left = node;
        node->parent = rightChild;
    }

    void rotateRight(Node* node) {
        Node* leftChild = node->left;
        node->left = leftChild->right;
        if (leftChild->right != nullptr)
            leftChild->right->parent = node;
        leftChild->parent = node->parent;
        if (node->parent == nullptr)
            root = leftChild;
        else if (node == node->parent->left)
            node->parent->left = leftChild;
        else
            node->parent->right = leftChild;
        leftChild->right = node;
        node->parent = leftChild;
    }

    void fixInsert(Node* node) {
        while (node != root && node->parent->color == Color::RED) {
            if (node->parent == node->parent->parent->left) {
                Node* uncle = node->parent->parent->right;
                if (uncle != nullptr && uncle->color == Color::RED) {
                    node->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        rotateLeft(node);
                    }
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    rotateRight(node->parent->parent);
                }
            } else {
                Node* uncle = node->parent->parent->left;
                if (uncle != nullptr && uncle->color == Color::RED) {
                    node->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rotateRight(node);
                    }
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    rotateLeft(node->parent->parent);
                }
            }
        }
        root->color = Color::BLACK;
    }

public:
    RedBlackTree() : root(nullptr) {}

    void insert(int key) {
        Node* newNode = new Node(key);
        Node* parent = nullptr;
        Node* current = root;
        while (current != nullptr) {
            parent = current;
            if (newNode->key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        newNode->parent = parent;
        if (parent == nullptr)
            root = newNode;
        else if (newNode->key < parent->key)
            parent->left = newNode;
        else
            parent->right = newNode;

        fixInsert(newNode);
    }

    void inorderTraversal(Node* node) {
        if (node == nullptr)
            return;
        inorderTraversal(node->left);
        std::cout << node->key << " ";
        inorderTraversal(node->right);
    }

    void inorder() {
        inorderTraversal(root);
    }
};

int main() {
    RedBlackTree tree;

    // Insert nodes into the tree
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(60);
    tree.insert(70);

    // Print the tree in order
    std::cout << "Inorder traversal: ";
    tree.inorder();
    std::cout << std::endl;

    return 0;
}

