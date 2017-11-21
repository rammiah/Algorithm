#ifndef SORT_H
// #include <vector>
#include <algorithm>
// #include <array>
#include <iostream>
namespace Sort{
    template<typename T>
    static void swap(T &a, T &b) {
        T t = a;
        a = b;
        b = t;
    }

    template<typename T>
    static void maxheapify(T *arr, const int &len, const int &index) {
        int largest = index;
        int temp = index;
        // 今天写个非递归的
        do {
            temp = largest;
            int l = 2 * temp + 1;
            int r = 2 * temp + 2;
            if (l < len && arr[l] > arr[largest]) largest = l;
            if (r < len && arr[r] > arr[largest]) largest = r;
            if (largest != temp) {
                swap<T>(arr[largest], arr[temp]);
                // 此时largest是最小
            }
        } while(largest != temp);
    }

    template<typename T>
    static void build_heap(T *&arr, const int &len) {
        for (int t = len / 2 - 1; t >= 0; --t) {
            maxheapify<T>(arr, len, t);
        }
    }

    template<typename T>
    void max_heap_sort(T *&arr, const int &len) {
        build_heap<T>(arr, len);
        for (int i = len - 1; i >= 1; --i) {
            // std::cout << arr[i] << " ";
            swap<T>(arr[i], arr[0]);
            maxheapify<T>(arr, i, 0);
        }
    }
}

#define SORT_H
#endif