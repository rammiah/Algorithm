// #include <iostream>
#include <cstdio>
#include <cmath>


int main() {
    int years;
    double rate[6] = {0};
    scanf("%d %lf %lf %lf %lf", &years, &rate[1], &rate[2], &rate[3], &rate[5]);
    double *money = new double[years + 1];
    money[0] = 1;
    double temp = 0;
    // 最基本的动态规划
    for (int i = 1; i <= years; ++i) {
        money[i] = money[i - 1] * (1 + rate[1]);
        for (int j = 1; j < 6; ++j) {
            // 幂函数是pow，记成exp了
            if (j <= i && (temp = money[i - j] * pow((rate[j] + 1), j)) > money[i]) {
                money[i] = temp;
            }
        }
    }
    // 输出结果
    printf("%.5f\n", money[years]);

    delete[] money;

    return 0;
}