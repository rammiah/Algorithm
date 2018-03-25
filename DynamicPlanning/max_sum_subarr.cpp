#include <iostream>
#include <vector>
#include <algorithm>

int sum[1000];

int max_sum(std::vector<int> &nums) {
    int size = nums.size();

    sum[0] = std::max(0, nums[0]);
    for (int i = 1; i < size; ++i) {
        sum[i] = std::max(sum[i - 1] + nums[i], nums[i]);
    }

    return sum[size - 1];
}


int main() {
    std::vector<int> nums{1, -2, 3, -1, 7};
    std::cout << max_sum(nums) << "\n";

    return 0;
}