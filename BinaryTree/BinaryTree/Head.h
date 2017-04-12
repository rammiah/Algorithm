#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Node
{
private:
	int value;
	Node* left;
	Node* right;
	int count;
public:
	Node(int value);
	void Insert(int value);
	void Insert(vector<int> arr);
	void Check();
	int GetCount();
};