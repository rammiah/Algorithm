/*
题目描述 
给你一个长度为n且由a和b组成的字符串，你可以删除其中任意的部分（可以不删），使得删除后的子串“变化”次数小于等于m次且最长。
变化：如果a[i]!=a[i+1]则为一次变化。（且新的字符串的首字母必须是'a'）
如果初始串全为b，则输出0。
输入描述:
第一行输入两个数n,m。（1 <= n <= 105,0 <= m <= 10）
第二行输入一行长度为n且由a和b组成的字符串
输出描述:
输出一个数字表示最长长度
示例1
输入
8 2
aabbabab
输出
6
说明
原串可以变成aabbbb，只改变了一次，且长度最长。
*/

#include <cstdio>
#include <algorithm>

using namespace std;

char str[100005];
int dp[100005][12];

void update(int &x, int b) {
    x = max(x, b);
}


int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    scanf("%s", str);
    // 怎么又是动态规划？？？
    // 需要打表吗
    char c;
    for (int i = 0; i < n; ++i) {
        if (str[i] == 'a') update(dp[i + 1][0], 1);
        for (int j = 0; j <= m; ++j) {
            if (j & 1) c = 'b';
            else c = 'a';

            if (dp[i][j] == 0) {
                continue;
            }

            update(dp[i + 1][j], dp[i][j]);
            if (str[i] == c) {
                update(dp[i + 1][j], dp[i][j] + 1);
            } else {
                update(dp[i + 1][j + 1], dp[i][j] + 1);
            }
        }
    }

    int ans = 0;
    for (int i = 0; i <= m; ++i) {
        update(ans, dp[n][i]);
    }
    printf("%d\n", ans);

    return 0;
}
