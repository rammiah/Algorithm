#include <iostream>
#include <vector>

// 注意，此代码没有考虑元素重复的情况

void search_all(std::vector<int> &value, std::vector<int> &in, const int &index) {
    // 搜索达到临界点
    if (index == value.size()) {
        // do something
        // 输出元素，其中in[i]记录value[i]是否属于当前子集
         for (int i = 0; i < value.size(); ++i) {
             if (in[i] == 1) {
                 std::cout << value[i] << " ";
             }
         }
        std::cout << "\n";
        // 结束递归
        return;
    }
    // 将当前点拿出子集，向下搜索
    in[index] = 0;
    search_all(value, in, index + 1);
    // 将点放入子集，搜索
    in[index] = 1;
    search_all(value, in, index + 1);
}

// 主函数
int main(int argc, char *argv[]) {
    // 声明变量和标记变量
    std::vector<int> v{1, 2, 3, 4, 5};
    // 全部初始化为
    std::vector<int> in(v.size(), 0);
    // 搜索
    search_all(v, in, 0);
    return 0;
}
