#include <iostream>
#include <limits.h>
#include <cstdio>
// 6x6的图，从1开始
int map[7][7];

void Dijkstra() {
    int start = 1;
    int current = start;
    bool visited[7] = {};
    int nearest = 0;
    int length = 0;
    visited[1] = true;
    int count = 6;
    while (count > 0) {
        visited[current] = true;
        length= INT_MAX;
        for (int i = 1; i < 7; ++i) {
            if (!visited[i] && map[i][current] < INT_MAX && map[i][start] > map[current][start] + map[current][i]) {
                map[i][start] = map[start][i] = map[current][start] + map[current][i];
            }
        }
        for (int i = 1; i < 7; ++i) {
            if (!visited[i] && map[i][start] < length) {
                nearest = i;
                length = map[i][start];
            }
        }

        current = nearest;
        --count;
    }
}

int main() {
    FILE* file = fopen("data.txt", "r");

    std::cout << "Hello, World!" << std::endl;
    std::cout << "From point 1" << std::endl;
    int roads = 9;
    int pointA = 0, pointB = 0;
    int distance = 0;

    for (int i = 1; i < 7; ++i) {
        for (int j = 1; j < 7; ++j) {
            map[i][j] = INT_MAX;
        }
        map[i][i] = 0;
    }
    for (int i = 0; i < roads; ++i) {
        fscanf(file, "%d %d %d", &pointA, &pointB, &distance);
        map[pointA][pointB] = map[pointB][pointA] = distance;
    }
    fclose(file);

    Dijkstra();

    for (int i = 1; i < 7; ++i) {
        std::cout << "1 to " << i << " is " << map[1][i] << " ";
        std::cout << std::endl;
    }

    return 0;
}
