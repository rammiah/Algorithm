#include<iostream>
using namespace std;
//获取父节点
int Parent(int i)
{
	return i / 2;
}
//获取左孩子
int Left(int i)
{
	return 2 * i;
}
//获取右孩子
int Right(int i)
{
	return 2 * i + 1;
}
//从i节点开始生成最大堆
void MaxHeap(int *a, int i, int length)
{
	int L, R;
	L = Left(i);
	R = Right(i);
	int largest;
	if (L <= length&&a[L - 1]>a[i - 1])
	{
		largest = L;
	}
	else
		largest = i;
	if (R <= length&&a[R - 1]>a[largest - 1])
	{
		largest = R;
	}
	if (largest != i)
	{
		int temp;
		temp = a[i - 1];
		a[i - 1] = a[largest - 1];
		a[largest - 1] = temp;
		MaxHeap(a, largest, length);
	}
}
//将整个树生成最大堆
void Build_Max_Heap(int *a, int length)
{
	//从length/2开始是因为length/2节点以下的都是叶子节点
	for (int i = length / 2; i >= 1; i--)
	{
		MaxHeap(a, i, length);
	}
}
//堆排序
void HeapSort(int *a, int length)
{
	Build_Max_Heap(a, length);
	int number = length;
	for (int i = length; i>0; i--)
	{
		int temp;
		temp = a[i - 1];
		a[i - 1] = a[0];
		a[0] = temp;
		length -= 1;
		MaxHeap(a, 1, length);
	}
}
int main()
{
	int a[10] = { 4,1,3,2,16,9,10,14,8,7 };
	HeapSort(a, 10);
	for (int i = 0; i<10; i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}