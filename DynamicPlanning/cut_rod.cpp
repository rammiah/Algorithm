// 问题描述：剪钢板，不同长度价格不同，怎么剪总价值最大
#include <iostream>
#include <string>
#include <algorithm>

constexpr int size = 11;

int values[size] = { 0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30 };

// 根据给定的大小计算可以得到的最大价值，动态规划
int max_value(int count) {
    auto help_arr = new int[count + 1];
    help_arr[0] = 0;
    int max = 0;
    for (int i = 1; i <= count; ++i) {
        max = 0;
        // 根据已知计算现在的
        for (int j = 1; j <= i; ++j) {
            max = std::max(max, help_arr[i - j] + values[j]);
        }
        help_arr[i] = max;
    }
    delete(help_arr);

    return max;
}

int main(int argc, char *argv[]) {
    int cnt = 0;
    std::cout << "Please enter size of rod: ";
    std::cin >> cnt;

    std::cout << "max value: " << max_value(cnt) << "\n";

    return 0;
}