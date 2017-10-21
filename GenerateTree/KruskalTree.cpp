#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h>

// 大部分都一样，我就直接粘贴啦
std::vector<std::vector<float>> graph;
// std::vector<std::>
// store the edges
std::vector<std::pair<int, int>> tree;
// std::vector<bool> used;

bool in_same_graph(const std::vector<int> &flag, const int &a, const int &b) {
    if (flag[a] == -1 || flag[b] == -1) {
        return false;
    }
    return flag[a] == flag[b];
}

void input_graph(const std::string file_name) {
    std::fstream in;
    in.open(file_name, std::ios_base::in);
    int v_count = 0, e_count = 0;
    in >> v_count >> e_count;
    graph = std::vector<std::vector<float>>(v_count, std::vector<float>(v_count, -1));
    // used = std::vector<bool>(v_count, false);
    int v1, v2;
    float distance = 0;
    for (int i = 0; i < e_count; ++i) {
        in >> v1 >> v2 >> distance;
        graph[v1][v2] = graph[v2][v1] = distance;
    }
    in.close();
}

float kruskal_tree() {
    int v1 = 0;
    int v2 = 0;

    int v_count = graph.size();
    int min = 0;
    std::vector<int> flag(v_count, -1);
    int index = 0;

    float cost = 0;
    tree.clear();
    while (tree.size() < v_count - 1) {
        min = INT_MAX;
        for (int i = 0; i < v_count; ++i) {
            for (int j = 0; j < v_count; ++j) {
                if (graph[i][j] > 0 && !in_same_graph(flag, i, j)) {
                    if (graph[i][j] < min) {
                        min = graph[i][j];
                        // std::cout << i << "->"<< j << "=" << min << std::endl;
                        v1 = i;
                        v2 = j;
                    }
                }
            }
        }
        // std::cout << std::endl;
        // used[v1] = used[v2] = true;
        // 维护他们所属的图
        if (flag[v1] == -1 && flag[v2] == -1) {
            flag[v1] = flag[v2] = index++;
        } else if (flag[v1] == -1) {
            flag[v1] = flag[v2];
        }else if (flag[v2] == -1){
            flag[v2] = flag[v1];
        } else {
            int max = std::max(flag[v1], flag[v2]);
            int min = std::min(flag[v1], flag[v2]);
            for (int i = 0; i < v_count; ++i) {
                if (flag[i] == max) {
                    flag[i] = min;
                }
            }
        }
        cost += graph[v1][v2];
        tree.push_back(std::make_pair(v1, v2));
    }

    return cost;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "No file name input" << std::endl;
        exit(EXIT_SUCCESS);
    }
    std::string file_name = argv[1];
    input_graph(file_name);
    float cost = kruskal_tree();
    std::cout << "cost = " << cost << std::endl;
    for (const std::pair<int, int> &t : tree) {
        std::cout << t.first << "->" << t.second << ":" << graph[t.first][t.second] << std::endl;
    }

    return 0;
}
