#include"Head.h"
Node::Node(int value)
{
	this->value = value;
	left = NULL;
	right = NULL;
}

void Node::Insert(int value)
{
	if (value == this->value)
	{
		return;
	}
	else if (value > this->value)
	{
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