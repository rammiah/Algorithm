#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>

template<typename V = int, int MAX_LEVEL = 10>
class SkipList {
private:
    // 如果直接存储v的话和set有什么区别呢
    struct ListNode {
        // 保存的值
        int key;
        std::shared_ptr<V> value;
        // 层数，不知道有什么用
        int level;
        ListNode **forwards;

        ListNode(int key, V val, int lev) : key(key), value(std::make_shared<V>(val)), level(lev) {
            forwards = new ListNode *[lev]{}; // 顺便初始化为空指针
        }

        ListNode(int key, int lev) : key(key), value(nullptr), level(lev) {
            forwards = new ListNode *[lev]{}; // 顺便初始化为空指针
        }

        ~ListNode() {
            delete[] forwards;
        }
    };

    // 这个val没什么用
    ListNode *head, *tail;
    size_t _size = 0;

    //  还要专门给个结束节点呢

    static int get_level() {
        int lev = 1;
        while (random() % 2) {
            lev++;
        }
        return std::min(MAX_LEVEL, lev);
    }

    // 返回<=key的最大key对应的node及路径，类似于lower_bound
    ListNode *find_node(int key, std::vector<ListNode *> &nodes) {
        ListNode *node = head;
        while (node != tail) {
            // 不为tail是因为tail的forwards全部为空指针，不可访问
            if (node->key == key) {
                // 找到节点key相同，直接替换值即可
                return node;
            }
            int lev = node->level - 1;
            while (lev >= 0 && node->forwards[lev]->key > key) {
                // 保存节点和层数
                nodes.emplace_back(node);
                --lev;
            }
            if (lev < 0) {
                // 新建节点去吧
                return nullptr;
            }
            node = node->forwards[lev];
        }
        // 插入操作总应该是成功的
        // 不应该运行到此处
        assert(false);
        return nullptr;
    }

public:
    SkipList() {
        head = new ListNode(INT_MIN, MAX_LEVEL);
        tail = new ListNode(INT_MAX, MAX_LEVEL);
        // 设置一下指针
        for (int i = 0; i < head->level; i++) {
            head->forwards[i] = tail;
        }
    }

    // 跳表插入操作
    // 插入是不可能失败的，插入返回true表示插入新节点，返回false表示更新旧节点
    bool insert(int key, V val) {
        // 接下来是插入新值的常规操作
        // 先获取一个高度
        int lev = get_level();
        // 对节点进行插入操作
        // 查找比key小的最大的节点，DONE: 新节点高度比搜索出的高怎么办? 保存搜索路径
        std::vector<ListNode *> nodes;
        ListNode *prev = find_node(key, nodes);
        // 如果找到了等key节点，直接更新原来值
        if (prev != nullptr) {
            // 更新旧节点的值
            *prev->value = val;
            return false;
        }
        if (nodes.size() != MAX_LEVEL) {
            for (int i = 0; i < nodes.size(); ++i) {
                std::cout << nodes[i] << std::endl;
            }
        }
        assert(nodes.size() == MAX_LEVEL);
        // 需要维持跳表结构
        // 应该倒着遍历vector
        auto *node = new ListNode(key, val, lev);
        for (int i = 1; i <= node->level; ++i) {
            node->forwards[i - 1] = nodes[MAX_LEVEL - i]->forwards[i - 1];
            nodes[MAX_LEVEL - i]->forwards[i - 1] = node;
        }
        ++_size;

        return true;
    }

    std::shared_ptr<V> find(int key) const {
        // 查询操作
        // 生命周期问题。。。返回智能指针好了
        ListNode *node = head;

        while (node != nullptr) {
            // 节点是否等于数据
            if (node->key == key) {
                return node->value;
            }
            // 记录高度，从高往低找
            int lev = node->level - 1;
            while (lev >= 0 && node->forwards[lev]->key > key) {
                --lev;
            }
            // 查找失败
            if (lev < 0) {
                return nullptr;
            }
            node = node->forwards[lev];
        }

        return nullptr;
    }


    bool remove(int key) {
        std::vector<ListNode *> nodes;
        ListNode *node = find_node(key, nodes);
        if (node == nullptr) {
            // key不存在
            return false;
        }
        // 删除节点
        int lev = node->level - 1;
        ListNode *prev;
        // 查找前置节点
        // 为空表示head一步直接找到，prev就是head
        if (nodes.empty()) {
            prev = head;
        } else {
            prev = nodes.back();
        }
        for (int i = lev; i >= 0; --i) {
            // 下层节点的问题，需要向后挪动
            while (prev->forwards[i] != node) {
                prev = prev->forwards[i];
            }
            prev->forwards[i] = node->forwards[i];
        }
        // 删除节点即可
        delete (node);
        --_size;
        return true;
    }

    // 遍历操作
    void traverse(const std::function<void(V)> &func) {
        ListNode *node = head->forwards[0];
        while (node != tail) {
            func(node->value);
            node = node->forwards[0];
        }
    }

    // 线性查找
    std::shared_ptr<V> linear_find(int key) const {
        ListNode *node = head->forwards[0];
        while (node != tail) {
            if (node->key == key) {
                return node->value;
            }
            node = node->forwards[0];
        }
        return nullptr;
    }

    size_t size() const {
        return this->_size;
    }
};

int main() {
    SkipList<> lst;
    constexpr int limit = 100000;
    constexpr int search_cnt = limit / 100;
    std::random_device dev;
    std::default_random_engine eng(dev());
    std::uniform_int_distribution<int> uni(0, limit);
    for (int i = 0; i < limit; ++i) {
        int k = uni(eng);
        lst.insert(k, k * 10);
    }

    // 尝试查找测试
    std::cout << "start search" << std::endl;
    time_t now = clock();
    for (int i = 0; i < search_cnt; i++) {
        int key = uni(eng);
        auto val = lst.find(key);
        if (val != nullptr && *val != key * 10) {
            std::cout << "find failed" << std::endl;
        }
    }
    std::cout << "time use: " << (clock() - now) << std::endl;
    // 尝试查找测试
    std::cout << "start linear search" << std::endl;
    now = clock();
    for (int i = 0; i < search_cnt; i++) {
        int key = uni(eng);
        auto val = lst.linear_find(key);
        if (val != nullptr && *val != key * 10) {
            std::cout << "find failed" << std::endl;
        }
    }
    std::cout << "time use: " << (clock() - now) << std::endl;
    // 测试删除
    for (int i = 0; i < search_cnt; i++) {
        int key = uni(eng);
        auto val = lst.find(key);
        if (val != nullptr) {
            if (!lst.remove(key)) {
                std::cout << "remove failed" << std::endl;
            }
        }
        val = lst.find(key);
        if (val != nullptr) {
            std::cout << "delete failed" << std::endl;
        }
    }

    return 0;
}
