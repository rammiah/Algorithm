#include <iostream>
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
vector<bool> is_in_set;  // 记录是否在result set里的vector
vector<float> delta;  // 就是那个delta数组
int current_size = 0;  // 当前set的大小
float current_cost = 0;  // 当前的cost，就是f
vector<int> v_in_set;
vector<int> v_out_set;


// 输入完全图
void input_graph(const string &file_name) {
    fstream input;
    // 打开文件
    input.open(file_name, ios_base::in);
    // 输入顶点数和要得到的集合的大小
    input >> v_count >> require_size;
    // 得到图
    graph = std::vector<vector<float>>(v_count, vector<float>(v_count, 0));
    is_in_set = vector<bool>(v_count, false);
    delta = vector<float>(v_count, 0);
    // 在swap里用于保存点
    v_in_set = vector<int>(require_size, -1);
    v_out_set = vector<int>(v_count - require_size, -1);

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
    if (is_in_set[v]) {
        cerr << "Error: " << v << " should not in set.\n";
        return false;
    } else {
        // 放入set
        is_in_set[v] = true;
        ++current_size;
        // 处理delta
        for (int i = 0; i < v_count; ++i) {
            if (is_in_set[i]) {
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
        if (is_in_set[i]) {
            max_delta = max(max_delta, delta[i]);
            min_delta = min(delta[i], min_delta);
        }
    }
    // 返回cost
    return max_delta - min_delta;
}


bool remove_from_set(int v) {
    if (!is_in_set[v]) {
        cerr << "Error: " << v << " should in set.\n";
        return false;
    } else {
        // 移出set
        is_in_set[v] = false;
        --current_size;
        // 此顶点的delta直接置为0
        delta[v] = 0;
        // 处理delta
        for (int i = 0; i < v_count; ++i) {
            if (is_in_set[i]) {
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
        if (is_in_set[i]) {
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
        if (is_in_set[i]) {
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
    is_in_set[v_in] = false;
    float out_cost = get_after_swap_cost(v_out, v_in);
    is_in_set[v_in] = true;
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
        if (is_in_set[i]) {
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
    // 随机加入一个
    add_to_set(static_cast<int>(rand() % v_count));
    int min_v = 0;
    float min_cost, temp_cost = 0;
    vector<int> all_best;
    // 贪心算法加入剩余的点
    while (current_size < require_size) {
        // 每次都要设置为FLOAT_MAX
        min_cost = FLOAT_MAX;
        all_best.clear();
        for (int i = 0; i < v_count; ++i) {
            // 如果i不在里面得到加入后其delta
            if (!is_in_set[i]) {
                // 获取加入此定点会得到的cost
                temp_cost = get_after_add_cost(i);
                if (temp_cost < min_cost) {
                    // 如果可以更小就更新min_v
//                    min_v = i;
                    min_cost = temp_cost;
                    // 如果当前的最小将all清空
                    all_best.clear();
                    all_best.push_back(i);
                } else if (abs(temp_cost - min_cost) < 0.0001) {
                    // 否则加入all
                    all_best.push_back(i);
                }
            }
        }
        // 随机选择加入
        add_to_set(all_best[rand() % all_best.size()]);
        // 把cost更新一下
        current_cost = min_cost;
    }
}

// std::swap居然不让用
template <typename T>
void swap(T& a, T&b){
    T t = a;
    a = b;
    b = t;
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

    // 将点加入到v_in_set和v_out_set
    int in = 0, out = 0;
    for (int i = 0; i < v_count; ++i) {
        if (is_in_set[i]) {
            v_in_set[in++] = i;
        } else {
            v_out_set[out++] = i;
        }
    }
    // 存放点对
    vector<pair<int, int>> can_swap;

    while (improved) {
        // 在此置为false
        improved = false;
        // 置为0
        max_improve = 0;
        can_swap.clear();
        for (int i = 0; i < in; ++i) {
            for (int o = 0; o < out; ++o) {
                // 遍历所有的可交换点，取最优
                temp_improve = swap_cost(v_in_set[i], v_out_set[o]);
                // 注意这里是负数
                if (temp_improve < max_improve) {
                    // 更新improve和交换的顶点
                    max_improve = temp_improve;
                    improved = true;
                    // 取得两顶点
                    can_swap.clear();
                    can_swap.push_back(make_pair(i, o));
                } else if (abs(temp_improve - max_improve) < 0.0001) {
                    can_swap.push_back(make_pair(i, o));
                }
            }
        }

        //检测是否进行交换
        if (improved) {
            auto r = rand() % can_swap.size();
            remove_from_set(v_in_set[can_swap[r].first]);
            add_to_set(v_out_set[can_swap[r].second]);
            // swap点
            ::swap(v_in_set[can_swap[r].first], v_out_set[can_swap[r].second]);
            // 更新cost
            current_cost += max_improve;
        }
    }
}

// reset函数
void reset() {
    current_size = 0;
    current_cost = 0;
    for (int i = 0; i < v_count; ++i) {
        is_in_set[i] = false;
        delta[i] = 0;
    }
}

// 对运算的一个封装吧
void run() {
    get_init_sol();
    improve();
}

// 输出到文件
void print_to_file(const vector<Result> &result, const string &file_name) {
    fstream out;
    out.open(file_name, ios_base::out);
    out << require_size << "\n\n";
    // 写入文件
    // 先输出所有的
    out << "all result:\n\n";
    for (auto &item : result) {
        out << "v in set: ";
        for (const int &v : item.set) {
            out << v << " ";
        }
        out << endl;
        out << "time used: "<< item.used << "ms" << endl;
        out << "cost: " << item.cost << "\n\n";
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
    out << "best result:\n";
    out << "v in set: ";
    for (const int &v : result[best].set) {
        out << v << " ";
    }
    out << endl;
    out << "time used: " << result[best].used << "ms" <<  endl;
    out << "cost: " << result[best].cost << "\n\n";

    // 平均
    out << "average: \n";
    out << "average time: " << time_sum / result.size() << "ms" << endl;
    out << "average cost: "<<  cost_sum / result.size() << "\n";
    // 关闭文件
    out.close();
}

int run_case(const string &file_name) {
    // 输入图。每次内存都会自动分配
    input_graph(file_name);
    vector<Result> result;
    vector<int> set;
    for (int i = 0; i < 10; ++i) {
        reset();
        // 顾不了那么多格式了
        cout << "Start running for " << i + 1 << "th time\n";
        srand(static_cast<unsigned int>(clock()));
        // 计时
        time_t start = clock();
        // 运行
        run();
        // 结束计时
        time_t end = clock();
        // linux下得到的是微秒us，除以1000得到ms
        time_t used = (end - start) / 1000;
        cout << "End running for " << i + 1 << "th time\n";
        cout << "time using: " << used << "ms\n";
        // 清空set
        set.clear();
        for (int j = 0; j < v_count; ++j) {
            if (is_in_set[j]) {
                set.push_back(j);
            }
        }
        // 插入到后面
        result.emplace_back(set, used, get_cost());
    }

    // 输出到文件
    print_to_file(result, file_name + ".out");
}

// 每次直接测试所有的
int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Two few parameters\n";
        exit(EXIT_FAILURE);
    }

    string file_name = argv[1];
    fstream in;
    in.open(file_name, ios_base::in);
    // 检测打开状态
    if (!in.good()) {
        cerr << "Open " + file_name + " error!\n";
        exit(EXIT_FAILURE);
    }
    // 输入样例个数
    int case_count = 0;
    in >> case_count;
    // 测试
    for (int i = 0; i < case_count; ++i) {
        in >> file_name;
        cout << i + 1 << " running " + file_name + ":\n";
        run_case(file_name);
        cout << "\n";
    }

    return 0;
}
