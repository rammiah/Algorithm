#include <iostream>
#include <string>
#include <algorithm>

int dp[100][100];


int max_sub_polin(const std::string &str) {
    int length = str.length();
    for (int i = 0; i < length; ++i) {
        // 每个字符都是初始长度为1的回文字符串
        dp[i][i] = 1;
    }
    // 测试所有长度为i的回文串，显然i从2开始，最终为length
    for (int i = 1; i < length; ++i) {
        // 从0开始搜索
        for (int j = 0; j + i < length; ++j) {
            if (str[j] == str[j + i]) {
                // 取最大
                dp[j][j + i] = std::max(dp[j][j + i], dp[j + 1][j + i - 1] + 2);
            } else {
                // 取上个片段中较长的
                dp[j][j + i] = std::max(dp[j][j + i - 1], dp[j + 1][i + j]);
            }
        }
    }
    // 返回2个字符中间最长的回文串长度
    return dp[0][length - 1];
}

int main(int argc, char *argv[]) {
    // 回文子序列为carac，长度为5
    std::string str = "character";
    if (argc > 1) {
        str = argv[1];
    }
    // 输出序列
    std::cout << max_sub_polin(str) << "\n";

    return 0;
}
