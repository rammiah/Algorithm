
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <map>
#include <set>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <queue>
#include <fstream>
#include <cmath>
#include <climits>
#include <cfloat>
#include <cstdarg>
#include <cctype>

using namespace std;

string add(const string &a, const string &b) {
    int len_a = a.length(), len_b = b.length();
    string result;
    int idx_a = len_a - 1, idx_b = len_b - 1;
    int carry = 0;
    while (idx_a >= 0 || idx_b >= 0 || carry > 0) {
        carry += idx_a >= 0 ? a[idx_a--] - '0' : 0;
        carry += idx_b >= 0 ? b[idx_b--] - '0' : 0;
        result.push_back(carry % 10 + '0');
        carry /= 10;
    }
    reverse(result.begin(), result.end());
    return result;
}

string sub(const string &a, const string &b) {
//    cout << a << " " << b << "\n";
    // 输入请确保a >= b
    int len_a = a.length(), len_b = b.length();
    assert(len_a >= len_b);
    int idx_a = len_a - 1, idx_b = len_b - 1;
    int carry = 0;
    string res;
    while (idx_b >= 0 || carry > 0) {
        carry += idx_b >= 0 ? b[idx_b--] - '0' : 0;
        int num_a = a[idx_a--] - '0';
        if (num_a < carry) {
            num_a += 10;
            num_a -= carry;
            carry = 1;
        } else {
            num_a -= carry;
            carry = 0;
        }
        res.push_back(num_a + '0');
    }
    while (idx_a >= 0) {
        res.push_back(a[idx_a--]);
    }
    // 去除首部的0
    reverse(res.begin(), res.end());
    while (res.length() > 1) {
        if (res[0] == '0') {
            res = res.substr(1);
        } else {
            break;
        }
    }
    return res;
}

void add_zero(string &str, int n) {
    // 在str后添加n个0
    while (n--) {
        str.push_back('0');
    }
}

void remove_zero(string &str) {
    // 去除前置0
    while (str.length() > 1) {
        if (str[0] == '0') {
            str = str.substr(1);
        } else {
            break;
        }
    }
}

bool bigger(const string &a, const string &b) {
    if (a.length() > b.length()) {
        return true;
    } else if (a.length() < b.length()) {
        return false;
    }
    return a >= b;
}

string mul(const string &a, const string &b) {
    // 请确保参数都为非负数，我的函数会去除首部的零
    if (a.length() == 0 || b.length() == 0 || a == "0" || b == "0") {
        return "0";
    }
    // 两者都为有效的数字
    int len_a = a.length(), len_b = b.length();
    // 如果其中有一个的位数已经小于等于2位了
    if (len_a <= 2 && len_b <= 2) {
        return to_string(stoi(a) * stoi(b));
    } else if (len_a <= 2) {
        // 只需要将b分开
        string b1 = b.substr(0, len_b / 2);
        string b0 = b.substr(len_b / 2);
        // 后面需要加len_b - len_b / 2个0
        b1 = mul(b1, a);
        b0 = mul(b0, a);
        add_zero(b1, len_b - len_b / 2);
        string res = add(b1, b0);
        remove_zero(res); // 去除前置0
        return res;
    } else if (len_b <= 2) {
        // 将a分开
        // 只需要将b分开
        string a1 = a.substr(0, len_a / 2);
        string a0 = a.substr(len_a / 2);
        // 后面需要加len_a - len_a / 2 个0
        a1 = mul(a1, b);
        a0 = mul(a0, b);
        add_zero(a1, len_a - len_a / 2);
        string res = add(a1, a0);
        remove_zero(res);
        return res;
    }
    // 两者都没有达到结束的地方
    // 我的程序存储的高位在前面
    // 两者想要继续计算需要AD BC的阶数相同，不然不能使用这个算法
    // 阶数定为与较大的相同
    if (len_a > len_b) {
        // 高位部分得到的为 len / 2，3是1，4是2，低位是(len + 1) / 2，总是容易计算的
        if (len_b <= (len_a + 1) / 2) {
            // b的高位参与运算的将是0
            string A = a.substr(0, len_a / 2);
            string B = a.substr(len_a / 2);
            remove_zero(B);
            string D = b;
            string BD = mul(B, D);
            // 此处讨论B和A的大小
            if (bigger(B, A)) {
                string res = sub(BD, mul(sub(B, A), D));
                add_zero(res, (len_a + 1) / 2);
                res = add(res, BD);
                remove_zero(res);
                return res;
            } else {
                string res = add(BD, mul(sub(A, B), D));
                add_zero(res, (len_a + 1) / 2);
                res = add(res, BD);
                remove_zero(res);
                return res;
            }
        } else {
            string A = a.substr(0, len_a / 2);
            string B = a.substr(len_a / 2);
            int len = (len_a + 1) / 2;
            string C = b.substr(0, len_b - len);
            string D = b.substr(len_b - len);
            // 去除B，D的前置0
            remove_zero(B);
            remove_zero(D);
            string AC = mul(A, C);
            string BD = mul(B, D);
            string res = AC;
            // 奇数拓展len_a+1个0，偶数拓展len_a个0
            add_zero(res, (len_a + 1) / 2 * 2);
            res = add(res, BD);
            // 分类讨论A和B，C和D的大小
            string mid = add(AC, BD);
            if (bigger(A, B) && bigger(D, C)) {
                mid = add(mid, mul(sub(A, B), sub(D, C)));
            } else if (bigger(A, B) && bigger(C, D)) {
                mid = sub(mid, mul(sub(A, B), sub(C, D)));
            } else if (bigger(B, A) && bigger(D, C)) {
                mid = sub(mid, mul(sub(B, A), sub(D, C)));
            } else {
                mid = add(mid, mul(sub(B, A), sub(C, D)));
            }
            add_zero(mid, (len_a + 1) / 2);
            res = add(res, mid);
            remove_zero(res);
            return res;
        }
    } else if (len_a <= len_b) {
        // 高位部分得到的为 len / 2，3是1，4是2，低位是(len + 1) / 2，总是容易计算的
        if (len_a <= (len_b + 1) / 2) {
            // b的高位参与运算的将是0
            string C = b.substr(0, len_b / 2);
            string D = b.substr(len_b / 2);
            remove_zero(D);
            string B = a;
            string BD = mul(B, D);
            // 此处讨论B和A的大小
            if (bigger(D, C)) {
                string res = sub(BD, mul(sub(D, C), B));
                add_zero(res, (len_b + 1) / 2);
                res = add(res, BD);
                remove_zero(res);
                return res;
            } else {
                string res = add(BD, mul(sub(C, D), B));
                add_zero(res, (len_b + 1) / 2);
                res = add(res, BD);
                remove_zero(res);
                return res;
            }
        } else {
            string C = b.substr(0, len_b / 2);
            string D = b.substr(len_b / 2);
            int len = (len_b + 1) / 2;
            string A = a.substr(0, len_a - len);
            string B = a.substr(len_a - len);
            // 去除B，D的前置0
            remove_zero(B);
            remove_zero(D);
            string AC = mul(A, C);
            string BD = mul(B, D);
            string res = AC;
            // 奇数拓展len_b+1个0，偶数拓展len_b个0
            add_zero(res, (len_b + 1) / 2 * 2);
            res = add(res, BD);
            // 分类讨论A和B，C和D的大小
            string mid = add(AC, BD);
            if (bigger(A, B) && bigger(D, C)) {
//                cout << __LINE__ << "\n";
                mid = add(mid, mul(sub(A, B), sub(D, C)));
            } else if (bigger(A, B) && bigger(C, D)) {
//                cout << __LINE__ << "\n";

                mid = sub(mid, mul(sub(A, B), sub(C, D)));
            } else if (bigger(B, A) && bigger(D, C)) {
//                cout << __LINE__ << "\n";
                mid = sub(mid, mul(sub(B, A), sub(D, C)));
            } else {
//                cout << __LINE__ << "\n";
                mid = add(mid, mul(sub(B, A), sub(C, D)));
            }
            add_zero(mid, (len_b + 1) / 2);
            res = add(res, mid);
            remove_zero(res);
            return res;
        }
    }
}

class BigNum {
private:
    int sign; // 符号位
    string num; // 数据
public:
    BigNum(string &num) {
        if (isdigit(num[0])) {
            sign = 1;
        } else if (num[0] == '-') {
            sign = -1;
            num = num.substr(1);
        } else if (num[0] == '+') {
            sign = 1;
            num = num.substr(1);
        }
        // 去除首部的0，注意全为0的情况
        while (num.length() > 1) {
            if (num[0] == '0') {
                num = num.substr(1);
            } else {
                break;
            }
        }
        this->num = num;
    }

    BigNum() {
        sign = 1;
        num = "0";
    }

    friend istream &operator>>(istream &is, BigNum &num) {
        string num_str;
        is >> num_str;
        num = BigNum(num_str);
        return is;
    }

    friend ostream &operator<<(ostream &os, const BigNum &num) {
        if (num.num == "0") {
            os << "0";
            return os;
        } else if (num.sign == -1) {
            os << "-";
        }
        os << num.num;
        return os;
    }

    friend BigNum operator+(const BigNum &a, const BigNum &b) {
        string num = "0";
        BigNum res(num);
        if (a.sign == b.sign) {
            // 判断符号位
            res.sign = a.sign;
            // 相同直接加就行了
            // 两非负字符串相加
            res.num = add(a.num, b.num);
        } else {
            // 符号位不同按照绝对值大的算
            if (a.num.length() > b.num.length()) {
                res.sign = a.sign;
                res.num = sub(a.num, b.num);
            } else if (a.num.length() < b.num.length()) {
                res.sign = b.sign;
                res.num = sub(b.num, a.num);
            } else if (a.num > b.num) {
                res.sign = a.sign;
                res.num = sub(a.num, b.num);
            } else if (a.num < b.num) {
                res.sign = b.sign;
                res.num = sub(b.num, a.num);
            } else {
                // 两数相加为0
                res.sign = 1;
                res.num = "0";
            }
        }

        return res;
    }

    friend BigNum operator-(const BigNum &a, const BigNum &b) {
        BigNum res;
        if (a.sign != b.sign) {
            // 结果与a的符号相同
            res.sign = a.sign;
            // 结果的数值部分为直接相加
            res.num = add(a.num, b.num);
        } else {
            // 符号相同时需要考虑结果正负问题
            if (a.num.length() > b.num.length()) {
                // 结果与a同号
                res.sign = a.sign;
                res.num = sub(a.num, b.num);
            } else if (a.num.length() < b.num.length()) {
                // 结果与a异号
                res.sign = -a.sign;
                res.num = sub(b.num, a.num);
            } else if (a.num > b.num) {
                // 长度相同比较大小
                res.sign = a.sign;
                res.num = sub(a.num, b.num);
            } else if (a.num < b.num) {
                res.sign = -a.sign;
                res.num = sub(b.num, a.num);
            } else {
                // 结果为0
                res.sign = 1;
                res.num = "0";
            }
        }
        return res;
    }

    friend bool operator==(const BigNum &a, const BigNum &b) {
        return a.sign == b.sign && a.num == b.num;
    }

    friend BigNum operator*(const BigNum &a, const BigNum &b) {
        // 乘法操作使用分治法进行
        // 首先符号位就是两个参数的符号位的运算了
        BigNum res;
        if (a.sign == b.sign) {
            res.sign = 1;
        } else {
            res.sign = -1;
        }
        // 下面是数字部分的运算
        // 分治相乘的算法
        res.num = mul(a.num, b.num);

        return res;
    }
};

int main() {
    ifstream in("big_num_bench.txt");
    if (!in.is_open()) {
        return 0;
    }
    BigNum a, b, add_v, sub_v, mul_v;
    while (in >> a >> b >> add_v >> sub_v >> mul_v) {
        cout << (a + b == add_v) << (a - b == sub_v) << (a * b == mul_v) << "\n";
    }

    return 0;
}
