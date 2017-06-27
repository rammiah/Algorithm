/*************************************************************************
	> File Name: MaxHeapSort.c
	> Author: Yaning Wang, CS1607
	> Mail: wangyanign6166@gmail.com
	> School: HUST
	> Created Time: 2017年06月08日 星期四 17时04分53秒
 ************************************************************************/

#include <stdio.h>

int Parent(int i) {
    return i / 2;
}

int Left(int i) {
    return 2 * i;
}

int Right(int i) {
    return 2 * i + 1;
}

void Swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void MaxHeapify(int array[], int node, int length) {
    int largest = node;
    int left = Left(node);
    int right = Right(node);
    // 取出结点子结点中较大的
    if (left <= length && array[left] > array[largest]) largest = left;
    if (right <= length && array[right] > array[largest]) largest = right;
    if (largest != node) {
        Swap(&array[largest], &array[node]);
        // 递归把最大元素往堆顶送
        MaxHeapify(array, largest, length);
    }
}

// 建堆
void BuildHeap(int array[], int length) {
    for (int i = length / 2; i >= 1; --i) {
        MaxHeapify(array, i, length);
    }
}

// 先建堆，再把堆顶的元素送到最后，
// 将换过来的堆顶元素堆化，注意堆的大小减1
void MaxHeapSort(int array[], int length) {
    BuildHeap(array, length);
    for (int i = length; i >= 1; --i) {
        Swap(&array[1], &array[i]);
        MaxHeapify(array, 1, i - 1);
    }
    printf("\n");
}

int main() {
    int array[] = {-1, 1, 3, 5, 2, 7, 6, 8};
    MaxHeapSort(array, 7);
    
    for (int i = 1; i <= 7; ++i) {
        printf("%d ", array[i]);
    }
    
    return 0;
}
