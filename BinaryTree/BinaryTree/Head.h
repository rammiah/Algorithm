#pragma once
#include <iostream>
#include <string>
using namespace std;
class Node
{
private:
	int value;
	Node* left;
	Node* right;
public:
	Node(int value);
	void Insert(int value);
	void Check();
};