#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <cstring>
#include <cctype>
#include <cmath>
#include <climits>
#include <algorithm>
#include <iostream>

using ull = unsigned long long;
using ll = long long;
using uint = unsigned int;

void update(int &x, int y) {
    x = std::min(x, y);
}

int get_minimum(const std::vector<int> &vec) {
    using std::vector;
    int size = vec.size() - 1;
    vector<vector<int>> dp(size, vector<int>(size, INT_MAX));
    for (int i = 0; i < size; ++i) {
        dp[i][i] = 0;
    }

    for (int len = 1; len < size; ++len) {
        for (int i = 0; i < size - len; ++i) {
            for (int j = i; j < i + len; ++j) {
                update(dp[i][i + len],
                    dp[i][j] + dp[j + 1][i + len] + vec[i] * vec[i + len + 1] * vec[j + 1]);
            }
        }
    }

    return dp[0][size - 1];
}

int main() {
    std::vector<int> matric = {5, 10, 3, 12, 5, 50, 6};

    std::cout << get_minimum(matric) << "\n";

    return 0;
}