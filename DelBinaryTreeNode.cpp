/*************************************************************************
	> File Name: DelBinaryTreeNode.cpp
	> Author: Yaning Wang, CS1607
	> Mail: wangyanign6166@gmail.com
	> School: HUST
	> Created Time: 2017年06月07日 星期三 19时21分55秒
 ************************************************************************/

/* 如果是叶子结点，直接删除
 * 
 * 如果有一个子结点，把子结点值复制过来
 * 再递归删除子结点
 * 
 * 如果有两个子结点，将右结点的值复制过来
 * ，递归删除右结点。
 * */

#include<iostream>
using std::cin;
using std::cout;
using std::endl;

struct Node{
    int value;
    Node*left;
    Node*right;
    Node(int value) {
        this->value=value;
        this->left = nullptr;
        this->right = nullptr;
    }
};

void AddNode(Node*root, int value) {
    if (root->value == value) {
        return;
    } else if (root->value > value) {
        if (root->left == nullptr) {
            root->left = new Node(value);
        }else {
            AddNode(root->left, value);
        }
    }else {
        if (root->right == nullptr) {
            root->right = new Node(value);
        } else {
            AddNode(root->right, value);
        }
    }
}

void MidPre(Node*root) {
    if (root == nullptr) {
        return;
    }
    MidPre(root->left);
    cout << " " << root->value;
    MidPre(root->right);
}

void DeleteNode(Node*parent, Node*root, int value) {
    if (root->value == value) {
        Node*to_del = root;
        if (root->left == nullptr && root->right == nullptr) {
            if (parent->left == root) parent->left = nullptr;
            else parent->right = nullptr;
            delete to_del;
        } else if (root->right == nullptr) {
            root->value = root->left->value;
            DeleteNode(root, root->left, root->value);
        } else {
            root->value = root->right->value;
            DeleteNode(root, root->right, root->value);
        }
    } else if (root->value > value) {
        DeleteNode(root, root->left, value);
    } else{
        DeleteNode(root, root->right, value);
    }
} 


int main(void) {
    Node*root = new Node(0);
    AddNode(root, 10);
    AddNode(root, 28);
    AddNode(root, 39);
    AddNode(root, 17);
    AddNode(root, 5);
    AddNode(root, 13);
    AddNode(root, 15);
    MidPre(root);
    cout << endl;
    DeleteNode(nullptr, root,15);
    MidPre(root);
    cout << endl;

    return 0;
}
