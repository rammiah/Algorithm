#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;
// 从Java里得到的32bit float最大值
const float FLOAT_MAX = 3.4028235E38f;
const clock_t MICRO_SECOND = 1000;
const clock_t SECOND = 1000 * MICRO_SECOND;

// 存放结果的结构体
struct Result {
    vector<int> set;
    time_t used;
    float cost;

    // 构造函数，便于push_back
    Result(vector<int> set, time_t used, float cost) : set(std::move(set)), used(used), cost(cost) {}
};

// 基本变量
size_t require_size;  // 要求的set_size
vector<vector<float>> graph;  // 存放距离的图
size_t v_count;  // 图的顶点个数
vector<bool> is_in_set;  // 记录是否在result set里的vector
vector<float> delta;  // 就是那个delta数组
int current_size = 0;  // 当前set的大小
float current_cost = 0;  // 当前的cost，就是f
vector<int> v_in_set; // 存放在集合中的顶点
vector<int> v_out_set; // 存放不在其中的顶点
// 扰动所用变量
size_t current_iter = 0;
// vector<size_t> taboo;
float best_cost = FLOAT_MAX;
vector<int> best; // 存放最佳的结果
// vector<int> temp_best;

// 用于get_cost计算
// vector<float> temp_delta;

// 设置此空间保存random前的状态
namespace before_cond {
    vector<float> delta;
    vector<int> v_in_set, v_out_set;
    vector<bool> is_in_set;
    // vector<int> best;// best直接全局，不记录
    float current_cost;
}

clock_t end_t;

// 输入完全图
void input_graph(const string &file_name) {
    fstream input;
    // 打开文件
    input.open(file_name, ios_base::in);
    // 输入顶点数和要得到的集合的大小n
    input >> v_count >> require_size;
    // 得到图
    graph = std::vector<vector<float>>(v_count, vector<float>(v_count, 0));
    // 计算时主要操作的几个vector
    is_in_set.resize(v_count, false);
    delta.resize(v_count, 0);
    v_in_set.resize(require_size);
    v_out_set.resize(v_count - require_size);
    best.resize(require_size);
    // 对暂存状态初始化
    // before_cond::delta.resize();
    before_cond::v_in_set.resize(require_size);
    before_cond::v_out_set.resize(v_count - require_size);
    // before_cond::best.resize(require_size);
    before_cond::delta.resize(v_count);
    before_cond::is_in_set.resize(v_count);

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

template<typename T>
void copy(const vector<T> &src, vector<T> &dst) {
    int len = src.size();
    for (int i = 0; i < len; ++i) {
        dst[i] = src[i];
    }
}

bool add_to_set(const size_t &v) {
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
    // 防止误差过大，在此将cost从点开始计算出来
    // 双层循环即可
    for (int i = 0; i < v_count; ++i) {
        delta[i] = 0;
    }
    // 把前require个位置占了放delta
    for (int i = 0; i < require_size; ++i) {
        for (int j = 0; j < require_size; ++j) {
            // 选用best中的点
            delta[i] += graph[best[i]][best[j]];
        }
    }

    float max_delta = 0, min_delta = FLOAT_MAX;
    for (int i = 0; i < require_size; ++i) {
        // 在set里就计算是否是max或者min
        max_delta = max(max_delta, delta[i]);
        min_delta = min(delta[i], min_delta);
    }
    // 返回cost
    return max_delta - min_delta;
}


bool remove_from_set(const size_t &v) {
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
float get_after_add_cost(const int &v_to_in) {
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

// 计算如果交换cost会减少多少
// 参数原为直接的点，不是下标，现在替换为下标
// random里请不要使用此函数，如果不下降此函数会直接返回1
float swap_cost_if_descend(const int &v_in_index, const int &v_out_index) {
    float v_delta = 0;
    float temp_delta = 0;
    float max_delta = 0, min_delta = FLOAT_MAX;
    // O(1)也优化一下
    int v_out = v_out_set[v_out_index], v_in = v_in_set[v_in_index];
    for (int i = 0; i < require_size; ++i) {
        if (i != v_in_index) {
            v_delta += graph[v_out][v_in_set[i]];
            temp_delta = delta[v_in_set[i]] - graph[v_in][v_in_set[i]] + graph[v_out][v_in_set[i]];
            max_delta = max(temp_delta, max_delta);
            min_delta = min(temp_delta, min_delta);
            // 不会下降的，跳出循环
            if (max_delta - min_delta > current_cost) {
                // 节约时间，直接返回1
                return 1;
            }
        }
    }
    // 如果不下降返回1,下降返回下降的值
    max_delta = max(v_delta, max_delta);
    min_delta = min(v_delta, min_delta);
    // 返回差值
    return (max_delta - min_delta) - current_cost;
}

// 测试时输出数据看的
void printSet() {
    cout << endl;
    cout << "cost: " << current_cost << endl;
    cout << "get_cost(): " << get_cost() << endl;
    // get_cost();
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
    add_to_set(rand() % v_count);
    // int min_v = 0;
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
        add_to_set(static_cast<size_t>(all_best[rand() % all_best.size()]));
    }
    // 把cost更新一下       
    current_cost = min_cost;
}

//// std::swap居然不让用
namespace me {
    template<typename T>
    void swap(T &a, T &b) {
        T t = a;
        a = b;
        b = t;
    }
}
clock_t in_descend = 0;

// 对当前进行持续优化的函数
void descend() {
    // clock_t s = clock();
    // 是否提升的一个flag
    bool improved = true;
    // 提升的值，已有最大和当前点
    float max_improve = 0;
    float temp_improve = 0;
    // 存放点对的下标
    vector<pair<int, int>> can_swap;

    while (improved) {
        // 在此置为false
        improved = false;
        // 置为0
        max_improve = 0;
        can_swap.clear();
        for (int i = 0; i < require_size; ++i) {
            for (int o = 0; o < v_count - require_size; ++o) {
                // 遍历所有的可交换点，取最优,传参为下标
                temp_improve = swap_cost_if_descend(i, o);
                // 注意这里是负数
                if (temp_improve < max_improve) {
                    // 更新improve和交换的顶点
                    max_improve = temp_improve;
                    improved = true;
                    // 取得两顶点下标
                    can_swap.clear();
                    can_swap.push_back(make_pair(i, o));
                } else if (abs(temp_improve - max_improve) < 0.0001) {
                    // 取得下标
                    can_swap.push_back(make_pair(i, o));
                }
            }
        }

        //检测是否进行交换
        if (improved) {
            auto r = rand() % can_swap.size();
            // can_swap存放的是下标
            remove_from_set(static_cast<size_t>(v_in_set[can_swap[r].first]));
            add_to_set(static_cast<size_t>(v_out_set[can_swap[r].second]));
            // swap点
            me::swap(v_in_set[can_swap[r].first], v_out_set[can_swap[r].second]);
            // 更新cost
            current_cost += max_improve;
        }
    }

    if (current_cost < best_cost) {
        // best = v_in_set;
        // 拷贝到best里面，直接调用复制构造太慢了
        copy(v_in_set, best);
        best_cost = current_cost;
        end_t = clock();
    }
    // in_descend += clock() - s;
}


// 定向扰动
//TODO
void directed() {
    // 先将顶点分开
    int in = 0, out = 0;
    for (int i = 0; i < v_count; ++i) {
        if (is_in_set[i]) {
            v_in_set[in++] = i;
        } else {
            v_out_set[out++] = i;
        }
    }
    // 定向扰动
}

// 求交换的cost，就算cost为正也不终止，用于randomed或者directed
float get_swap_cost(const size_t &v_in_i, const size_t &v_out_i) {
    // 进入此函数时，已经完成初始化，现对其进行优化
    float v_delta = 0;
    float max_delta = 0, min_delta = FLOAT_MAX;
    float temp_delta = 0;
    int v_in = v_in_set[v_in_i], v_out = v_out_set[v_out_i];

    for (int i = 0; i < require_size; ++i) {
        if (v_in_set[i] != v_in) {
            temp_delta = delta[i] + graph[v_out][v_in_set[i]] - graph[v_in][v_in_set[i]];
            v_delta += graph[v_out][v_in_set[i]];
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

//namespace before_cond{
//     vector<float> delta;
//     vector<int>v_in_set, v_out_set;
//     vector<bool> is_in_set;
//     vector<int> best;
//     float current_cost;
// }

// 保存状态
void save() {
    copy(delta, before_cond::delta);
    copy(v_in_set, before_cond::v_in_set);
    copy(v_out_set, before_cond::v_out_set);
    copy(is_in_set, before_cond::is_in_set);
    before_cond::current_cost = current_cost;
}

// 恢复状态
void recover() {
    copy(before_cond::delta, delta);
    copy(before_cond::v_in_set, v_in_set);
    copy(before_cond::v_out_set, v_out_set);
    copy(before_cond::is_in_set, is_in_set);
    current_cost = before_cond::current_cost;
}

// 随机扰动
void randomed(const size_t &iters) {
    size_t to_out = 0, to_in = 0;
    int t = 0;
    int nbrs = static_cast<int>(0.2 * require_size);
    while (t < iters) {
        for (int j = 0; j < nbrs; ++j) {
            // to_out可以直接随机选
            to_out = rand() % require_size;
            to_in = rand() % (v_count - require_size);
            // 计算cost
            float cost = get_swap_cost(to_out, to_in);
            // 删除、加入
            remove_from_set(static_cast<size_t>(v_in_set[to_out]));
            add_to_set(static_cast<size_t>(v_out_set[to_in]));
            me::swap(v_in_set[to_out], v_out_set[to_in]);
            // 更新cost
            current_cost += cost;
        }
        // 下降
        descend();
        // 如果改进了那么做出的改变不进行恢复
        if (current_cost < best_cost) {
            copy(v_in_set, best);
  //          save();
            best_cost = current_cost;
            t = 0;
            end_t = clock();
        } else {
            t++;
        }
    }
}

// 对运算的一个封装吧
void run() {
    get_init_sol();
    int in = 0, out = 0;
    for (int i = 0; i < v_count; ++i) {
        if (is_in_set[i]) {
            v_in_set[in++] = i;
        } else {
            v_out_set[out++] = i;
        }
    }
    descend();

    clock_t start = clock();
    // stop单位为us
    clock_t stop = static_cast<clock_t>(start + v_count * SECOND);
    save();
    while (clock() < stop) {
        // 恢复已知最好状态
//        recover();
        randomed(10);
    }
}

// 输出到文件
void print_to_file(const vector<Result> &result, const string &file_name) {
    fstream out;
    out.open(file_name, ios_base::out);
//    out << require_size << "\n\n";
    // 写入文件
    // 先输出所有的
    out << "all result:\n";
    for (auto &item : result) {
        out << "v in set: ";
        for (const int &v : item.set) {
            out << v << " ";
        }
        out << endl;
        out << "time used: " << item.used << "ms" << endl;
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
    out << "time used: " << result[best].used << "ms" << endl;
    out << "cost: " << result[best].cost << "\n\n";

    // 输出到命令行一下
    cout << "Best cost: " << result[best].cost << "\n";

    // 平均
    out << "average: \n";
    out << "average time: " << time_sum / result.size() << "ms" << endl;
    out << "average cost: " << cost_sum / result.size() << "\n";
    // 关闭文件
    out.close();
}


// reset函数
void reset() {
    current_size = 0;
    current_cost = 0;
    best_cost = FLOAT_MAX;
    current_iter = 0;
    // in_rand = in_descend = 0;
    for (int i = 0; i < v_count; ++i) {
        is_in_set[i] = false;
        delta[i] = 0;
    }
}

// 封装
void run_case(const string &file_name) {
    // 输入图。每次内存都会自动分配
    input_graph(file_name);
    vector<Result> result;
//    vector<int> set;
    //TODO
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

        // linux下得到的是微秒us，除以1000得到ms
        time_t used = (end_t - start) / 1000;
        cout << "End running for " << i + 1 << "th time\n";
        cout << "Time using: " << used << "ms\n";
        cout << "Best cost: " << best_cost << "\n";
        // 排序，加入到result里面
        sort(best.begin(), best.end());
        result.emplace_back(best, used, get_cost());
    }

    // 输出到文件
    print_to_file(result, file_name + ".out.txt");
}

// 每次直接测试所有的
// 还是加参数吧，0表示测试文件列表，1表示直接测试数据
int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Two few parameters\n";
        exit(EXIT_FAILURE);
    }
    string s(argv[1]);
    if (s == "0") {
        string file_name = argv[2];
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
    } else {
        run_case(argv[2]);
    }

    return 0;
}
