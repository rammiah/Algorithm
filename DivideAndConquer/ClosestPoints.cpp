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

using namespace std;

struct Point {
    int x;
    int y;
};

bool operator<(const Point &a, const Point &b) {
    if (a.x < b.x) {
        return true;
    } else if (a.x > b.x) {
        return false;
    }
    return a.y < b.y;
}

set<pair<Point, Point>> res;

bool cmpx(const Point &a, const Point &b) {
    // 对x坐标从小到大排序的函数
    return a.x < b.x;
}

bool cmpy(const Point &a, const Point &b) {
    // 对y坐标进行排序
    return a.y < b.y;
}

double get_dis(const Point &a, const Point &b) {
    // 返回两者之间的欧几里得距离
    return hypot(a.x - b.x, a.y - b.y);
}

void merge(vector<Point> &points, int left, int mid, int right) {
    // 普通的合并操作
    static vector<Point> temp;
    temp.clear();
    int l = left, r = mid;
    while (l < mid && r < right) {
        if (cmpy(points[l], points[r])) {
            temp.push_back(points[l++]);
        } else {
            temp.push_back(points[r++]);
        }
    }
    while (l < mid) {
        temp.push_back(points[l++]);
    }
    while (r < right) {
        temp.push_back(points[r++]);
    }
    for (int i = 0; i < temp.size(); ++i) {
        points[i + left] = temp[i];
    }
}

// 提供的区间为前闭后闭类型
void get_min(const vector<Point> &points, vector<Point> &points_y, int left, int right, double &closest) {
    if (left >= right) {
        return;
    } else if (left + 3 >= right) {
        // 最小情况，暴力求解
        for (int i = left; i < right; ++i) {
            for (int j = i + 1; j < right; ++j) {
                double dis = get_dis(points[i], points[j]);
                if (dis < closest) {
                    closest = dis;
                    res.clear();
                    res.insert(make_pair(points[i], points[j]));
                } else if (dis == closest) {
                    res.insert(make_pair(points[i], points[j]));
                }
            }
        }
        sort(points_y.begin() + left, points_y.begin() + right, cmpy);
        return;
    }

    // 开始日常操作，分治开始
    int mid = (left + right) / 2;
    get_min(points, points_y, left, mid, closest);
    get_min(points, points_y, mid, right, closest);
    // 开始计算mid两侧的点的最短距离了
    // 归并排序的归并部分，时间复杂度O(n)
    merge(points_y, left, mid, right);
    // 找出x方向上符合点
    vector<Point> order_y;
    for (int i = left; i < right; ++i) {
        if (abs(points_y[i].x - points[mid].x) < closest) {
            order_y.push_back(points_y[i]);
        }
    }
    double dis;
    for (int i = 0; i < order_y.size(); ++i) {
        // 此处最多向后查询8个
        for (int j = i + 1; j < order_y.size(); ++j) {
            // 结束点
            if (order_y[j].y - order_y[i].y > closest) {
                //cout << j - i << "\n";
                break;
            }
            dis = get_dis(order_y[i], order_y[j]);
            if (dis < closest) {
                closest = dis;
                res.clear();
                res.insert(make_pair(order_y[i], order_y[j]));
            } else if (dis == closest) {
                res.insert(make_pair(order_y[i], order_y[j]));
            }
        }
    }
}

double get_min_tradition(const vector<Point> &points) {
    // 暴力方式
    double res = DBL_MAX;

    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            res = std::min(get_dis(points[i], points[j]), res);
        }
    }
    return res;
}

int main() {
    ifstream in("in.dat");
    ofstream out("out.dat");
    stdout;
    int cnt;
    in >> cnt;
    vector<Point> points;
    int x, y;
    for (int i = 0; i < cnt; ++i) {
        in >> x >> y;
        points.push_back({x, y});
    }
    in.close();
    sort(points.begin(), points.end(), cmpx);
    auto points_y = points;

    double dis = DBL_MAX;
    get_min(points, points_y, 0, cnt, dis);
    for (auto &pts : res) {
        out << pts.first.x << " " << pts.first.y << " " << pts.second.x << " " << pts.second.y << "\n";
    }
    out.close();

    return 0;
}
