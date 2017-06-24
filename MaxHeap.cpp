/*************************************************************************
	> File Name: MaxHeap.cpp
	> Author: Yaning Wang, CS1607
	> Mail: wangyanign6166@gmail.com
	> School: HUST
	> Created Time: 2017年06月07日 星期三 19时08分15秒
 ************************************************************************/

#include<iostream>
using std::cin;
using std::cout;
using std::endl;

int Parent(int i ){
    return i / 2;
}

int Left(int i) {
    return 2 * i;
}

int Right(int i) {
    return 2 * i + 1;
}

void Swap(int&a, int&b){
    int t = a;
    a = b;
    b = t;
}

void MaxHeap(int*heap, int node, int length) {
    int largest = node;
    int L = Left(node);
    int R = Right(node);
    if (L <= length && heap[L] > heap[largest])largest = L;
    if (R <= length && heap[R] > heap[largest])largest = R;
    
    if (largest != node) {
        Swap(heap[largest], heap[node]);
        MaxHeap(heap, largest, length);
    }
}

void BuildHeap(int*heap, int length) {
    for (int i = length / 2; i >= 1; --i) {
        MaxHeap(heap, i, length);
    }
}

int main(void) {
    int count = 0;
    cin >> count;

    int*heap = new int[count + 1];
    for (int i = 1; i <= count; ++i) {
        cin >> heap[i];
    }
    BuildHeap(heap, count);
    for(int i = 1; i <= count; ++i) {
        cout << heap[i] << " ";
    }
    cout << endl;
	// 忘了释放空间
	delete[] heap;
	
    return 0;
}
