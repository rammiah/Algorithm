#include <iostream>
//#include <utility>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;
// 从Java里得到的32bit float最大值
const float FLOAT_MAX = 3.4028235E38f;

// 存放结果的结构体
struct Result {
    vector<int> set;
    time_t used;
    float cost;

    // 构造函数，便于push_back
    Result(vector<int> set, time_t used, float cost) : set(std::move(set)), used(used), cost(cost) {}
};

int require_size;  // 要求的set_size
vector<vector<float>> graph;  // 存放距离的图
size_t v_count;  // 图的顶点个数
vector<bool> in_set;  // 记录是否在result set里的vector
vector<float> delta;  // 就是那个delta数组
int current_size = 0;  // 当前set的大小
float current_cost = 0;  // 当前的cost，就是f


// 输入完全图
void input_graph(const string &file_name) {
    fstream input;
    // 打开文件
    input.open(file_name, ios_base::in);
    // 输入顶点数和要得到的集合的大小
    input >> v_count >> require_size;
    // 得到图
    graph = std::vector<vector<float>>(v_count, vector<float>(v_count, 0));
    in_set = vector<bool>(v_count, false);
    delta = vector<float>(v_count, 0);
    int from, to;
    // 距离
    float distance;
    // 边的个数
    size_t e_count = v_count * (v_count - 1) / 2;
    // 输入边
    for (int i = 0; i < e_count; ++i) {
        input >> from >> to >> distance;
        graph[from][to] = graph[to][from] = distance;
    }
    // 关闭文件
    input.close();
}

bool add_to_set(int v) {
    // 不合理就判错
    if (in_set[v]) {
        cerr << "Error: " << v << " should not in set.\n";
        return false;
    } else {
        // 放入set
        in_set[v] = true;
        ++current_size;
        // 处理delta
        for (int i = 0; i < v_count; ++i) {
            if (in_set[i]) {
                delta[i] += graph[v][i];
                // 不要忘记v的delta，v == i时为0,不需要特殊处理
                delta[v] += graph[v][i];
            }
        }
        // 返回值无用
        return true;
    }
}

// 获取当前的cost
float get_cost() {
    // 记录下max和min
    float max_delta = 0, min_delta = FLOAT_MAX;
    for (int i = 0; i < v_count; ++i) {
        // 在set里就计算是否是max或者min
        if (in_set[i]) {
            max_delta = max(max_delta, delta[i]);
            min_delta = min(delta[i], min_delta);
        }
    }
    // 返回cost
    return max_delta - min_delta;
}


bool remove_from_set(int v) {
    if (!in_set[v]) {
        cerr << "Error: " << v << " should in set.\n";
        return false;
    } else {
        // 移出set
        in_set[v] = false;
        --current_size;
        // 此顶点的delta直接置为0
        delta[v] = 0;
        // 处理delta
        for (int i = 0; i < v_count; ++i) {
            if (in_set[i]) {
                delta[i] -= graph[v][i];
            }
        }
        // 其实这个返回值也是没啥意义
        return true;
    }
}

// 此处用于贪心插入顶点用
float get_after_add_cost(int v_to_in) {
    float v_delta = 0;
    float max_delta = 0, min_delta = FLOAT_MAX;
    float temp_delta = 0;
    // 遍历在set中的顶点，计算max和min
    for (int i = 0; i < v_count; ++i) {
        if (in_set[i]) {
            v_delta += graph[v_to_in][i];
            // 得到加入v后i的delta
            temp_delta = delta[i] + graph[v_to_in][i];
            max_delta = max(temp_delta, max_delta);
            min_delta = min(temp_delta, min_delta);
        }
    }
    // 在这里处理一下v_to_in
    max_delta = max(v_delta, max_delta);
    min_delta = min(v_delta, min_delta);
    return max_delta - min_delta;
}

// 和上面add那个很像，不过需要减去v_to_out到i的距离
float get_after_swap_cost(int v_to_in, int v_to_out) {
    float v_delta = 0;
    float max_delta = 0, min_delta = FLOAT_MAX;
    float temp_delta = 0;
    for (int i = 0; i < v_count; ++i) {
        if (in_set[i]) {
            v_delta += graph[v_to_in][i];
            // 更新delta
            temp_delta = delta[i] + graph[v_to_in][i] - graph[v_to_out][i];
            // 更新最大和最小
            max_delta = max(temp_delta, max_delta);
            min_delta = min(temp_delta, min_delta);
        }
    }
    // 在这里处理一下
    max_delta = max(v_delta, max_delta);
    min_delta = min(v_delta, min_delta);
    // 返回cost
    return max_delta - min_delta;
}

// 计算如果交换cost会减少多少
float swap_cost(const int &v_in, const int &v_out) {
    in_set[v_in] = false;
    float out_cost = get_after_swap_cost(v_out, v_in);
    in_set[v_in] = true;
    // 返回差值
    return out_cost - current_cost;
}

// 测试时输出数据看的
void printSet() {
    cout << endl;
    cout << "cost: " << current_cost << endl;
    cout << "get_cost(): " << get_cost() << endl;
    get_cost();
    int out_count = 0;
    // 得到的set
    for (int i = 0; i < v_count; ++i) {
        if (in_set[i]) {
            cout << i << " ";
            out_count++;
            // 换行
            if (out_count % 10 == 0) {
                cout << endl;
            }
        }
    }

    cout << endl;
}

// 初始化
void get_init_sol() {
    // 先把0加进去
    add_to_set(rand() % v_count);
    int min_v = 0;
    float min_cost = FLOAT_MAX, temp_cost = 0;
    // 贪心算法加入剩余的点
    while (current_size < require_size) {
        // 每次都要设置为FLOAT_MAX
        min_cost = FLOAT_MAX;
        for (int i = 0; i < v_count; ++i) {
            // 如果i不在里面得到加入后其delta
            if (!in_set[i]) {
                // 获取加入此定点会得到的cost
                temp_cost = get_after_add_cost(i);
                if (temp_cost < min_cost) {
                    // 如果可以更小就更新min_v
                    min_v = i;
                    min_cost = temp_cost;
                }
            }
        }
        // add在更新时计算了v_delta
        add_to_set(min_v);
        // 把cost更新一下
        current_cost = min_cost;
    }
//    cout << current_size << endl;
}

// 对当前进行持续优化的函数
void improve() {
    // 是否提升的一个flag
    bool improved = true;
    // 互相交换的顶点
    int v_to_in = 0, v_to_out = 0;
    // 提升的值，已有最大和当前点
    float max_improve = 0;
    float temp_improve = 0;

    while (improved) {
        // 在此置为false
        improved = false;
        // 置为0
        max_improve = 0;
        for (int in = 0; in < v_count; ++in) {
            if (in_set[in]) {
                for (int out = 0; out < v_count; ++out) {
                    // 遍历所有的可交换点，取最优
                    if (!in_set[out]) {
                        // 获取improve
                        temp_improve = swap_cost(in, out);
                        // 注意这里是负数
                        if (temp_improve < max_improve) {
                            // 更新improve和交换的顶点
                            max_improve = temp_improve;
                            improved = true;
                            // 取得两顶点
                            v_to_in = out;
                            v_to_out = in;
                        }
                    }
                }
            }
        }

        //检测是否进行交换
        if (improved) {
            // 替换swap函数
            remove_from_set(v_to_out);
            add_to_set(v_to_in);
            // 更新cost
//            cout << max_improve << endl;
            current_cost += max_improve;
        }
    }
}

// reset函数
void reset() {
    current_size = 0;
    current_cost = 0;
    for (int i = 0; i < v_count; ++i) {
        in_set[i] = false;
        delta[i] = 0;
    }

}

// 对运算的一个封装吧
void run() {
    get_init_sol();
    improve();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Too few arguments." << endl;
        exit(EXIT_FAILURE);
    }
    // 要输入的数据文件名称
    string file_name = argv[1];
    vector<vector<float>> graph;
    input_graph(file_name);

    vector<Result> result;
    vector<int> set;
    for (int i = 0; i < 10; ++i) {
        reset();
        // 顾不了那么多格式了
        cout << "Start running for " << i + 1 << "th times\n";
        // 计时
        time_t start = clock();
        // 运行
        run();
        // 结束计时
        time_t end = clock();
        // linux下得到的是微妙us，除以1000得到ms
        time_t used = (end - start) / 1000;
        cout << "End running for " << i + 1 << "th times\n";
        cout << "time using: " << used << "ms\n";
        // 清空set
        set.clear();
        for (int j = 0; j < v_count; ++j) {
            if (in_set[j]) {
                set.push_back(j);
            }
        }
        // 插入到后面
        result.emplace_back(set, used, current_cost);
    }

    file_name.append(".out");
    fstream out;
    out.open(file_name, ios_base::out);
    out << require_size << "\n\n";
    // 写入文件
    // 先输出所有的
    for (auto &item : result) {
        for (const int &v : item.set) {
            out << v << " ";
        }
        out << endl;
        out << item.used << endl;
        out << item.cost << "\n\n";
    }
    // 得到平均操作时间和cost，顺便记下最优cost的编号，就不能用foreach了
    int best = 0;
    float time_sum = 0, cost_sum = 0;

    for (int i = 0; i < result.size(); ++i) {
        time_sum += result[i].used;
        cost_sum += result[i].cost;
        if (result[i].cost < result[best].cost) {
            best = i;
        }
    }
    // 最优
    for (const int &v : result[best].set) {
        out << v << " ";
    }
    out << endl;
    out << result[best].used << endl;
    out << result[best].cost << "\n\n";

    // 平均
    out << time_sum / result.size() << endl;
    out << cost_sum / result.size() << "\n";
    // 关闭文件
    out.close();
    return 0;
}
