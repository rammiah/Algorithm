#include <vector>
#include <iostream>
#include <algorithm>
#define INF (1<<20)

using namespace std;

using Graph = vector<vector<int>>;

void floyd(Graph &g) {
    int vs = g.size();
    for (int i = 0; i < vs; ++i) {
        for (int j = 0; j < vs; ++j) {
            for (int k = 0; k < vs; ++k) {
                g[j][k] = min(g[j][k], g[j][i] + g[i][k]);
            }
        }
    }
}

int main() {
    freopen("graph.txt", "r", stdin);
    int vertex;
    int edges;
    cin >> vertex >> edges;
    Graph g(vertex, vector<int>(vertex, INF));
    int from, to, dis;
    while (edges--) {
        cin >> from >> to >> dis;
        // 双向
        g[from][to] = g[to][from] = dis;
    }

    floyd(g);
    // 输出信息
    for (int i = 0; i < vertex; ++i) {
        for (int j = i + 1; j < vertex; ++j) {
            if (g[i][j] != INF) {
                cout << i << " -> " << j << ": " << g[i][j] << ".\n";
            }
        }
    }

    return 0;
}