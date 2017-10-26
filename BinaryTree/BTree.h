// head file
#ifndef _BTREE_H_
#define _BTREE_H_

#include <iostream>
#include <vector>
// template<typename T>
// using func = void(const T &value);
template<typename T>
class BTree {
private:
    // 内部结构体
    // template<typename T>
    struct Node{
        T value;
        Node *left = nullptr;
        Node *right = nullptr;
        Node() = delete;
        // template<typename T>
        Node(const T &value):value(value), left(nullptr), right(nullptr){}
    };
    Node *root = nullptr;
    // template<typename T>
    void insertValue(const T &value, Node *&node);
    void travelTree(const Node *node);  
    bool deleteNode(const T &value, Node *&node);
    void freeTree(Node *&node);
public:
    // 默认构造函数
    BTree(){}
    // 给出root节点的值
    BTree(const T &value) {
        root = new Node(value);
    }
    // 给出初始节点的值的向量
    BTree(const std::vector<T> values) {
        for (const T &v : values) {
            insertValue(v);
        }
    }
    // 析构函数

    ~BTree();

    // 插入函数
    void insertValue(const T &value);
    // 查找函数
    bool searchValue(const T &value);
    // 删除函数
    bool deleteValue(const T &value);
    // 遍历函数
    void travelTree();
    // 带函数遍历// 学习一下如何传函数为参数
    // template<typename T>
    // void travelTree(func f);
};

// BTree::BTree() { }

template<typename T>
void BTree<T>::insertValue(const T &value) {
    insertValue(value, root);
}

template<typename T>
BTree<T>::~BTree() {
    freeTree(root);
}

template<typename T>
void BTree<T>::freeTree(Node *&node) {
    if (node == nullptr) {
        return;
    }
    freeTree(node->left);
    freeTree(node->right);
    delete node;
    node = nullptr;
}

// 插入元素
template<typename T>
void BTree<T>::insertValue(const T &value, Node *&node) {
    if (node == nullptr) {
        node = new Node(value);
    } else if (value < node->value) {
        insertValue(value, node->left);
    } else if (value > node->value) {
        insertValue(value, node->right);
    }
}

// 搜索元素
template<typename T>
bool BTree<T>::searchValue(const T &value) {
    // 非递归搜索
    Node *temp = root;
    while (temp != nullptr) {
        if (temp->value > value) {
            temp = temp->left;
        } else if (temp->value < value) {
            temp = temp->right;  
        } else {
            return true;
        }
    }

    return false;
}

// 删除元素
template<typename T>
bool BTree<T>::deleteValue(const T &value) {
    return deleteNode(value, root);
}

template<typename T>
bool BTree<T>::deleteNode(const T &value, Node *&node) {
    if (node == nullptr) {
        return false;
    } else if (value < node->value) {
        return deleteNode(value, node->left);
    } else if (value > node->value) {
        return deleteNode(value, node->right);        
    } else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        } else if (node->left == nullptr) {
            Node *t = node;
            node = node->right;
            delete t;
        } else if (node->right == nullptr) {
            Node *t = node;
            node = node->left;
            delete t;
        } else {
            Node *to_del = node->left, *before = node;
            while (to_del->right != nullptr) {
                before = to_del;
                to_del = to_del->right;
            }
            if (before == node) {
                delete node;                
                node = to_del;
            } else {
                node->value = to_del->value;
                before->right = to_del->left;
                delete to_del;
            }
        }
        return true;
    }
}

template<typename T>
void BTree<T>::travelTree() {
    // std::cout << "Go to travel\n"; 
    travelTree(root);
    // std::cout << "End travle\n";
}

template<typename T>
void BTree<T>::travelTree(const Node *node) {
    if (node == nullptr) {
        return;
    }
    // std::cout << "To left\n";
    travelTree(node->left);
    // std::cout << "Mid\n";
    std::cout << node->value << " ";
    // std::cout << "Right\n";
    travelTree(node->right);
}

#endif
