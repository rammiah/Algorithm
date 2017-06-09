/*************************************************************************
	> File Name: mergeSort.c
	> Author: Yaning Wang, CS1607
	> Mail: wangyanign6166@gmail.com
	> School: HUST
	> Created Time: 2017年06月09日 星期五 15时40分37秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

void merge(int a[], int first, int mid, int last) {
    int *temp = (int *) malloc((last - first + 1) * sizeof(int));
    int i = first, j = mid + 1;
    int k = 0;
    while (i <= mid && j <= last) {
        if (a[i] < a[j]) {
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
        }
    }
    while (i <= mid) temp[k++] = a[i++];
    while (j <= last) temp[k++] = a[j++];
    for (i = 0; i < k; ++i) {
        a[first + i] = temp[i];
    }
    free(temp);
}

void mergeSort(int a[], int first, int last) {
    if (first < last) {
        int mid = (first + last) / 2;
        mergeSort(a, first, mid);
        mergeSort(a, mid + 1, last);
        merge(a, first, mid, last);
    }
}

int main() {
    int arr[] = {1, 4, 7, 2, 4};
    mergeSort(arr, 0, 4);
    for (int i = 0; i < 5; ++i) {
        printf("%d ", arr[i]);
    }

    return 0;
}
