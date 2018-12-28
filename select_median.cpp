#include <condition_variable>
#include <mutex>
#include <iostream>
#include <new>
#include <alloca.h>
#include <vector>
#include <cassert>
#include <algorithm>

int partition(int *arr, int start, int end) {
    // 对[start, end)间的元素进行划分
    assert(start < end);
    int x = arr[end - 1];
    int i = start;
    for (int j = start; j < end - 1; ++j) {
        if (arr[j] <= x) {
            std::swap(arr[i], arr[j]);
            i++;
        }
    }
    std::swap(arr[end - 1], arr[i]);
    return i;
}

void sort(int arr[], int start, int end) {
    if (start >= end) {
        return;
    }
    int mid = partition(arr, start, end);
    sort(arr, start, mid);
    sort(arr, mid + 1, end);
}

int new_partion(int arr[], int start, int end, int pivot) {
    // 使用pivot对数据进行排序
    for (int i = start; i < end; ++i) {
        if (arr[i] == pivot) {
            std::swap(arr[i], arr[end - 1]);
            return partition(arr, start, end);
        }
    }
    return -1;
}

int simple_median(int arr[], int start, int end) {
    for (int i = start + 1; i < end; ++i) {
        int val = arr[i];
        int j = i;
        while (j >= start && arr[j] >= val) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j + 1] = val;
    }
    // 查找中位数
    return arr[(start + end) / 2];
}

int median(int arr[], int start, int end, int target) {
    if (end - start <= 5) {
        int mid = simple_median(arr, start, end);
        if (target >= start && target < end) {
            return arr[target];
        }
        // 返回中位数
        return mid;
    }
    // 划分
    // 最多会得到(end - start) / 5 + 1个中位数
    int max_cnt = (end - start) / 5 + 1;
    std::unique_ptr<int[]> ptr = std::make_unique<int[]>(max_cnt);
    int cnt = 0;
    int i;
    for (i = start; i < end - 5; i += 5) {
        ptr[cnt++] = simple_median(arr, i, i + 5);
    }
    ptr[cnt++] = simple_median(arr, i, end);
    // 得到中位数
    int mid = median(ptr.get(), 0, cnt, cnt / 2);
    int pivot = new_partion(arr, start, end, mid);
    if (pivot < target) {
        return median(arr, pivot + 1, end, target);
    } else if (pivot > target) {
        return median(arr, start, pivot, target);
    }
    return arr[pivot];
}

#define  N 200000
int arr[N];

int main() {
    srand(clock());
    std::generate(arr, arr + N, []() {
        return random();
    });
    // 输出中位数
    std::cout << median(arr, 0, N, N / 2) << std::endl;
    sort(arr, 0, N);
    std::cout << arr[N / 2] << std::endl;
//    for (int i = 0; i < N; ++i) {
//        std::cout << arr[i] << ", ";
//    }

    return 0;
}

