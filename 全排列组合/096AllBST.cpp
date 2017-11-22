class Solution {
public:
    int numTrees(int n) {
        int *arr = new int[n + 1];
        arr[0] = arr[1] = 1;

        for (int i = 2; i <= n; ++i) {
            arr[i] = 0;
            for (int j = 1; j <= i; ++j) {
                arr[i] += arr[j - 1] * arr[i - j];
            }
        }
        int t = arr[n];
        delete[] arr;
        return t;
    }
};
