#include <iostream>
#include <ctime>

// using namespace std;

void select_sort(int *arr, const int &size) {
    int min = 0;
    for (int i = 0; i < size; ++i) {
        min = i;
        if (arr[i] < arr[min]) {
            min = i;
        }
        if (min != i) {
            std::swap(arr[i], arr[min]);
        }
    }
}

int main(void) {
    srand(clock());
    constexpr int size = 100;
    int arr[size];
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1024;
    }
    select_sort(arr, size);

    return 0;
}
