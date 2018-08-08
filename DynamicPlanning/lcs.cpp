#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

int dp[1000][1000];
int record[1000];

// 求解最长公共子序列，使用dp啦
string lcs(string a, string b) {
    string res;
    memset(dp, 0, sizeof(dp));
    int a_len = a.length(), b_len = b.length();
    // 记录a中字符串的位置
    for (int i = 0; i < a_len; ++i) {
        dp[0][i] = a[i] == b[0] ? 1 : 0;
    }
    for (int i = 0; i < b_len; ++i) {
        dp[i][0] = a[0] == b[i] ? 1 : 0;
    }


    int max_len = 0;
    for (int i = 1; i < a_len; ++i) {
        for (int j = 1; j < b_len; ++j) {
            if (a[i] == b[j]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                // record[i] = i - 1;
            } else {
                // record[i] = dp[i - 1][j] > dp[i][j - 1] ? i - 1 : i;
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
            max_len = max(max_len, dp[i][j]);
            printf("%d ", dp[i][j]);
        }
        printf("\n");
    }
    // 开始构建结果字符串
    cout << max_len << "\n";
    int idx = 0;
    res.resize(max_len, ' ');
    // for (int i = 0; i < a_len; ++i) {
    //     if (dp[0][i] == 1) {
    //         res[idx++] = a[i];
    //         break;
    //     }
    // }
    int row = 1, col = 1;
    while (row < b_len && col < a_len) {
        if (dp[row][col] == dp[row - 1][col - 1]) {
            res[idx++] = a[col];
            row++;
            col++;
        } else {
            col++;
        }
    }

    return res;
}

int main() {
    string a = "1ewq234567891234";
    string b = "01234321234";

    cout << lcs(a, b) << "\n";

    return 0;
}
