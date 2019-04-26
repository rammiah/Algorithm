#include <iostream>
#include <stack>

using namespace std;

class BinaryTree {
public:
	bool insert(int val) {
		return insert(root, val);
	}

private:
	struct Node {
		int key;
		Node* left = nullptr, * right = nullptr;
		Node() = default;
		Node(int key) : key(key){
		}
	};
	// 使得迭代器可以访问内部元素
	friend class Iterator;

	Node* root = nullptr;

	bool insert(Node *&node, int val) {
		if (node == nullptr) {
			node = new Node(val);
			return true;
		}

		if (val < node->key) {
			return insert(node->left, val);
		}
		else if (val > node->key) {
			return insert(node->right, val);
		}
		return false;
	}
};
class Iterator {
private:
	typedef BinaryTree::Node Node;
	std::stack<Node*> nodes;

public:
	Iterator(BinaryTree& tree) {
		Node* root = tree.root;
		// 将其写入nodes
		while (root != nullptr) {
			nodes.push(root);
			root = root->left;
		}
	}

	bool valid() {
		// 不空就是有元素
		return !nodes.empty();
	}
	int operator*() {
		// 取得当前指针所指的值
		if (nodes.empty()) {
			throw std::exception("无元素可用");
		}
		return nodes.top()->key;
	}

	// 向下一个元素迭代
	Iterator& operator++() {
		if (nodes.empty()) {
			throw std::exception("无元素可用");
		}
		// 得到栈顶元素
		Node* node = nodes.top();
		nodes.pop();
		if (node->right != nullptr) {
			// 右侧不为空遍历右侧第一个及其左部到底
			node = node->right;
			while (node != nullptr) {
				nodes.push(node);
				node = node->left;
			}
		}

		return *this;
	}
};


int32_t main() {
	BinaryTree tree;
	tree.insert(10);
	tree.insert(20);
	tree.insert(15);
	tree.insert(25);
	tree.insert(5);
	tree.insert(7);
	tree.insert(3);


	Iterator iter{ tree };

	while (iter.valid()) {
		std::cout << *iter << "\n";
		++iter;
	}
	
	return 0;
}
