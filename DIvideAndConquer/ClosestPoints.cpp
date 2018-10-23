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
	double x;
	double y;
};
Point point_a, point_b;

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

// 提供的区间为前闭后闭类型
void get_min(const vector<Point> &points, int left, int right, double &closest) {
	if (left >= right) {
		return;
	}
	// 只剩下2个点的情况
	else if (left == right - 1) {
		double dis = get_dis(points[left], points[right]);
		if (dis < closest) {
			closest = dis;
			point_a = points[left];
			point_b = points[right];
		}
		return;
	}
	// 还有3个点 
	else if (left == right - 2) {
		double d12 = get_dis(points[left], points[left + 1]);
		double d13 = get_dis(points[left], points[left + 2]);
		double d23 = get_dis(points[left + 1], points[left + 2]);
		// d12最小
		if (d12 < d13 && d12 < d23) {
			if (d12 < closest) {
				closest = d12;
				point_a = points[left];
				point_b = points[left + 1];
			}
			return;
		}
		// 不是d12就在它们俩中间
		if (d23 < d13) {
			// d23最短
			if (d23 < closest) {
				closest = d23;
				point_a = points[left + 1];
				point_b = points[left + 2];
			}
			return;
		}
		// 只能是d13
		if (d13 < closest) {
			closest = d13;
			point_a = points[left];
			point_b = points[right];
		}
		return;
	}
	// 开始日常操作，分治开始
	int mid = (left + right) / 2;
	get_min(points, left, mid, closest);
	get_min(points, mid + 1, right, closest);
	// 开始计算mid两侧的点的最短距离了
	// closest中存储了递归时的最小值
	vector<Point> order_y;
	// 这里的点按照y的方式从小到大排
	// 查找所有距离mid处点的x距离小于closest的点
	for (int i = left; i <= right; ++i) {
		if (abs(points[mid].x - points[i].x) <= closest) {
			order_y.push_back(points[i]);
		}
	}
	// 使用系统自带函数进行排序
	sort(order_y.begin(), order_y.end(), cmpy);
	double dis;
	for (int i = 0; i < order_y.size(); ++i) {
		if (order_y[i].x > points[mid].x) {
			continue;
		}
		// 其实应该把两者的点分开
		for (int j = i + 1; j < order_y.size(); ++j) {
			if (order_y[j].x <= points[mid].x) {
				continue;
			}
			if (order_y[j].x - order_y[i].x >= closest) {
				break;
			}
			dis = get_dis(order_y[i], order_y[j]);
			if (dis < closest) {
				closest = dis;
				point_a = order_y[i];
				point_b = order_y[j];
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
	int cnt = 0;
	std::cin >> cnt;
	double x, y;
	Point p;
	vector<Point> points;
	for (int i = 0; i < cnt; ++i) {
		p.x = rand();
		p.y = rand();
		points.push_back(p);
	}

	sort(points.begin(), points.end(), cmpx);

	double dis1 = DBL_MAX;
	get_min(points, 0, cnt - 1, dis1);
	double dis2 = get_min_tradition(points);

	std::cout << dis1 << " " << dis2 << "\n";

	return 0;
}

