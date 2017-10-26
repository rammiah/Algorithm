/*************************************************************************
	> File Name: DelBinaryTreeNode.cpp
	> Author: Yaning Wang, CS1607
	> Mail: wangyanign6166@gmail.com
	> School: HUST
	> Created Time: 2017年06月07日 星期三 19时21分55秒
 ************************************************************************/

#include<iostream>
using std::cin;
using std::cout;
using std::endl;

struct Node{
    int value;
    Node *left;
    Node *right;
    Node(int value) {
        this->value=value;
        this->left = nullptr;
        this->right = nullptr;
    }
};

void AddNode(Node *root, int value) {
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

void MidPre(Node *root) {
    if (root == nullptr) {
        return;
    }
    MidPre(root->left);
    cout << " " << root->value;
    MidPre(root->right);
}

void DeleteNode(Node *&toDel) {
    Node *s, *q = toDel;
    if (toDel->right == nullptr) {
        toDel = toDel->left;
        delete q;
    } else if (toDel->left == nullptr) {
        toDel = toDel->right;
        delete q;
    } else {
        s = toDel->left;
        while (s->right != nullptr) {
            q = s;
            s = s->right;
        }
        toDel->value = s->value;
        if (q != toDel) {
            q->right = s->left;
        } else {
            q->left = s->left;
        }
        delete s;
    }
}

void Delete(Node *&root, const int &value) {
    if (root == nullptr) {
        return;
    } else if (root->value > value) {
        Delete(root->left, value);
    } else if (root->value < value) {
        Delete(root->right, value);
    } else {
        DeleteNode(root);
    }

}

int main(void) {
    Node *root = new Node(0);
    AddNode(root, 10);
    AddNode(root, 28);
    AddNode(root, 39);
    AddNode(root, 17);
    AddNode(root, 5);
    AddNode(root, 13);
    AddNode(root, 15);
    MidPre(root);
    cout << endl;
    cout << "Del 15" << endl;
    Delete(root,15);
    MidPre(root);
    cout << endl;
    cout << "Del 13" << endl;
    Delete(root, 13);
    MidPre(root);
    cout << endl;

    return 0;
}
