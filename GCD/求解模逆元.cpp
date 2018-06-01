#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

int exgcd(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int r = exgcd(b, a % b, x, y);
    int t = x;
    x = y;
    y = t - a / b * y;
    return r;
}

int fast_pow(int a, int b, int mod) {
    int result = 1;
    while (b != 0) {
        if (b & 1) {
            result = result * a % mod;
        }
        a = a * a % mod;
        b >>= 1;
    }

    return result;
}

int main() {
    // 47X + 30Y = 1
    // int x, y;
    // cout << exgcd(47, 30, x, y);
    // x = -7, y = 11
    // x是47mod30模逆元，y是30mod47模逆元
    cout << fast_pow(47, 7, 30);

    return 0;
}