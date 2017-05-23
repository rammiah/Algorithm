#include "table.h"   
using namespace std;

int main()
{
	int len = 10;
	HashTable<int> hashTest(len);
	//一次插入元素    
	hashTest.insertHash(10);
	hashTest.insertHash(1);
	hashTest.insertHash(9);
	hashTest.insertHash(5);
	hashTest.insertHash(5);
	hashTest.insertHash(2);
	hashTest.insertHash(18);
	hashTest.insertHash(65);
	hashTest.insertHash(21);
	//输出哈希表中的元素    
	cout << "表中的元素为：   ";
	for (int i = 0; i<len; i++)
	{
		cout << hashTest.getData(i) << "  ";
	}
	cout << endl;
	//查找元素位置    
	cout << "元素9的位置：";
	int pos = hashTest.getHash(9);
	if (pos == -1)
	{
		cout << "无此元素" << endl;
	}
	else
	{
		cout << pos << endl;
	}
	system("pause");
	return 0;
}