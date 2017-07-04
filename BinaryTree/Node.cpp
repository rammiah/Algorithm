#include"Head.h"
Node::Node(int value)
{
	this->value = value;
	left = NULL;
	right = NULL;
	count = 1;
}

void Node::Insert(int value)
{
	if (value == this->value)
	{
		return;
	}
	else if (value > this->value)
	{
		++count;
		if (this->right == NULL)
		{
			this->right = new Node(value);
		}
		else
		{
			this->right->Insert(value);
		}
	}
	else
	{
		++count;
		if (this->left == NULL)
		{
			this->left = new Node(value);
		}
		else
		{
			this->left->Insert(value);
		}
	}
}
void Node::Insert(vector<int> arr)
{
	for each (auto item in arr)
	{
		Insert(item);
	}
}

void Node::Check()
{
	if (this == NULL)
	{
		return;
	}
	this->left->Check();
	cout << value << " ";
	this->right->Check();
}
int Node::GetCount() { return count; }