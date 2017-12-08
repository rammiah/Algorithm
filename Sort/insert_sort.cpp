#include <iostream>
#include <ctime>

void insert_sort(int *arr, const int &size) {
    int temp = arr[0];
    for (int i = 1; i < size; ++i) {
        int j = i - 1;
        temp = arr[i];
        while (j >= 0 && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

int main() {
    srand(clock());
    constexpr int size = 20;
    int arr[size];
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1024;
    }

    insert_sort(arr, size);

    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }

    return 0;
}