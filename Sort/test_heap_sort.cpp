#include <iostream>
#include "sort.h"

#define LEN 100

int main() {
    int *arr = new int[LEN];
    srand(clock());
    for (int i = 0; i < LEN; ++i) {
        arr[i] = rand() % 1024;
    }

    Sort::max_heap_sort<int>(arr, LEN);

    for (int i = 0; i < LEN; ++i) {
        std::cout << arr[i] << " ";
        if ((i + 1) % 10 == 0) {
            std::cout << "\n";
        } 
    }

    delete[] arr;
    return 0;
}
