#include <iostream>
#include <ctime>

void max_heapify(int *arr, const int &index, const int &size) {
    int largest = index;
    int left = (index<<1) + 1;
    int right = (index<<1) + 2;
    if (left < size && arr[left] > arr[largest]) largest = left;
    if (right < size && arr[right] > arr[largest]) largest = right;
    if (largest != index) {
        std::swap(arr[index], arr[largest]);
        max_heapify(arr, largest, size);
    }
}

void build_heap(int *arr, const int &size) {
    for (int i = size / 2; i >=0; --i) {
        max_heapify(arr, i, size);
    }
}

void heap_sort(int *arr, const int &size) {
    build_heap(arr, size);
    for (int i = size - 1; i >= 1; --i) {
        std::swap(arr[0], arr[i]);
        max_heapify(arr, 0, i);
    }
}

int main() {
     srand(clock());
    constexpr int size = 2000;
    int arr[size];
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1024;
    }

    heap_sort(arr, size);

    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }

    return 0;
}
