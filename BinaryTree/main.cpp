#include "Head.h"

int main(void)
{
	Node head(10);
	vector<int>arr = { 2, 13, 20, 17, 3, 15, 5, 93, 18, 22, 83 };
	head.Insert(arr);
	head.Check();
	cout << head.GetCount() << endl;
	return 0;
}