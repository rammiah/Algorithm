#include <iostream>

template<typename T>
class BST {
    struct Node {
        T value;
        int height = 1;

        Node *left = nullptr;
        Node *right = nullptr;

        explicit Node(const T &value) :
                value(value) {}
    };

    Node *root = nullptr;

    void adjHeight(Node *node) {
        if (node == nullptr) {
            return;
        }

        if (node->left == nullptr && node->right == nullptr) {
            node->height = 1;
        } else if (node->left == nullptr) {
            adjHeight(node->right);
            node->height = node->right->height + 1;
        } else if (node->right == nullptr) {
            adjHeight(node->left);
            node->height = node->left->height + 1;
        }
    }

    void RRotate(Node *&node) {
        Node *left = node->left;
        node->left = left->right;
        left->right = node;
        node = left;
    }

    void LRotate(Node *&node) {
        Node *right = node->right;
        node->right = right->left;
        right->left = node;
        node = right;
    }

    void LRRotate(Node *&node) {
        LRotate(node->left);
        RRotate(node);
    }

    void RLRotate(Node *&node) {
        RRotate(node->right);
        LRotate(node);
    }

    bool insert(const T &value, Node *&node) {
        if (node == nullptr) {
            node = new Node(value);
            return true;
        }

        if (value == node->value) {
            return false;
        }

        if (value > node->value) {
            insert(value, node->right);
        } else if (value < node->value) {
            insert(value, node->left);
        }

        int left_height = node->left != nullptr ? node->left->height : 0;
        int right_height = node->right != nullptr ? node->right->height : 0;
        // 不平衡了,左边比右边高两个
        if (left_height - right_height == 2) {
            // 插到左左
            if (value < node->left->value) {
                RRotate(node);
            } else {
                LRRotate(node);
            }
        } else
            // 右边比左边高两个
        if (left_height - right_height == -2) {
            if (value > node->right->value) {
                LRotate(node);
            } else {
                RLRotate(node);
            }
        }

        adjHeight(node);

        return true;
    }

    void InOrder(const Node *node) const {
        if (node != nullptr) {
            InOrder(node->left);
            std::cout << node->value << "\n";
            InOrder(node->right);
        }
    }

    bool balance(Node *node) {
        if (node == nullptr) {
            return true;
        }
        if (balance(node->left) && balance(node->right)) {
            int left_height = node->left != nullptr ? node->left->height : 0;
            int right_height = node->right != nullptr ? node->right->height : 0;

            return std::abs(left_height - right_height) < 2;
        }
        return false;
    }

public:
    bool insert(const T &value) {
        return insert(value, root);
    }

    void inOrder() const {
        InOrder(root);
    }

    bool balance() {
        return balance(root);
    }
};


int main() {

    for (int j = 0; j < 10; ++j) {

        std::cout << j + 1 << "th test:\n";
        BST<size_t> tree{};
        srand(clock());
        int size = 1000000;
        for (int i = 0; i < size; ++i) {
            tree.insert(static_cast<const size_t>(random() % 1024));
        }
        std::cout << "Tree is balance ? " << tree.balance() << "\n";
//        tree.inOrder();
    }


    return 0;
}