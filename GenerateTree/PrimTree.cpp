#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h>

std::vector<std::vector<float>> graph;
// store the edges
std::vector<std::pair<int, int>> tree;

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

float prim_tree() {
    // stop when tree.size == v_count - 1
    int v_count = graph.size();
    std::vector<bool> used(v_count, false);
    // start with v 0
    int v = 0;    
    used[0] = true;
    int min = 0;
    int to_add = 0;
    float cost = 0;
    tree.clear();
    while (tree.size() < v_count - 1) {
        min = INT_MAX;
        for (int i = 0; i < v_count; ++i) {
            // v = i;
            if (used[i]) {
                for (int j = 0; j < v_count; ++j) {
                    if (!used[j] && graph[i][j] > 0) {
                        if (graph[i][j] < min) {
                            min = graph[i][j];
                            v = i;
                            to_add = j;
                        }
                    }
                }
            }
        }
        used[to_add] = true;
        cost += graph[v][to_add];
        tree.push_back(std::make_pair(v, to_add));
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
    float cost = prim_tree();
    std::cout << "cost = " << cost << std::endl;
    for (const std::pair<int, int> &t : tree) {
        std::cout << t.first << "->" << t.second << ":" << graph[t.first][t.second] << std::endl;
    }

    return 0;
}
