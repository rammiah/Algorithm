#include <iostream>

using namespace std;

void QuickSort(int* arr, const int first, const int last) {
	if (first >= last) {
		return;
	}
	int temp = 0;
	int _fir = first;
	int _la = last;
	int key = arr[_fir];
	while (_fir < _la) {
		while (arr[_fir] < key) {
			++_fir;
		}
		while (arr[_la] > key) {
			--_la;
		}
		temp = arr[_fir];
		arr[_fir] = arr[_la];
		arr[_la] = temp;
	}
	arr[_la] = key;
	QuickSort(arr, first, _fir - 1);
	QuickSort(arr, _fir + 1, last);
}

int main() {
	int a[10] = { 3, 1, 5, 2, 7, 8, 9, 4, 6, 0 };
	QuickSort(a, 0, 9);

	for each (int i in a)
	{
		cout << i << " ";
	}


	return 0;
}
