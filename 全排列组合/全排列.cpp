#include <iostream>
#include <vector>
#include <algorithm>

// 将start及其后面元素对掉，形成最小的排列
void reverse(std::vector<int> &v, const int &start) {
    int e = v.size() - 1;
    int s = start;
    // 如果s和e间没有元素，结束函数
    if (s >= e) {
        return;
    }
    // 进行对掉
    while (s < e) {
        std::swap(v[s], v[e]);
        ++s;
        --e;
    }
}

// 得到下一个排列，原地进行，不返回新的向量
// 返回值true表示得到了下一个排列
bool next(std::vector<int> &v){
    int e = v.size() - 1;
    // 从后先前找到第一个不符合递减的数字
    while (e >= 1 && v[e] < v[e - 1]) --e;
    // e为0表示已经是最大的排列
    // 即[5, 4, 3, 2, 1]
    if (e == 0) {
        std::cerr << "No next permutations!" << std::endl;
        return false;
    }
    // 从后向前寻找地一个大于v[e - 1]的数字
    // 例[1, 2, 5, 4, 3],e就是元素2所在位置下标1，寻找到的last_bigger就是3对应的4
    int last_bigger = v.size() - 1;
    while (v[last_bigger] < v[e - 1]) --last_bigger;
    // 交换2,3
    std::swap(v[last_bigger], v[e - 1]);
    // 反转为[1, 3, 2, 4, 5],得到下一个排列
    reverse(v, e);
    return true;
}

int main(int argc, char *argv[]) {
    // 初始化变量
    std::vector<int> v{1, 2, 3, 4};
    // 输出当前序列
    std::for_each(std::cbegin(v), std::cend(v), [](const int &t){std::cout << t << "";});
    std::cout << "\n";
    // 按字典序持续得到所有排列并输出
    while (next(v)) {
        std::for_each(v.begin(), v.end(), [](int t){std::cout << t << "";});
        std::cout << "\n";      
    }
    
    return 0;
}
