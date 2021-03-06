#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <queue>
#include <map>


struct Node {
    // 存储的字符
    char ch;
    // 出现频率
    double frequency;
    // 左右指针
    Node *left = nullptr, *right = nullptr;

    // 默认建立的是最大堆，需要修改一下小于号的定义
    friend bool operator<(const Node &a, const Node &b) {
        return a.frequency > b.frequency;
    }

    // 构造函数
    Node() = delete;
    //使用字符和频率构造
    Node(char ch, double frequency) :ch(ch), frequency(frequency) {}
    // 只有频率，在合并中会使用
    Node(double frequency) : ch('\0'), frequency(frequency) {}
    // 拷贝构造，在new新节点时使用
    Node(const Node &node) : ch(node.ch), frequency(node.frequency),
        left(node.left), right(node.right) {}
};

// 遍历树，
void travelTree(const Node *node, const std::string &code) {
    // 问题是Huffman树是正则二元树，这个判断没啥用
    /*if (node == nullptr) {
        return;
    }*/

    // 左右为空是树叶，就是有字符的地方
    if (node->left == nullptr && node->right == nullptr) {
        // 输出字符和频率
        std::cout << "'" << node->ch << "': " << code << "\n";
        //std::cout << "Frequency: " << std::setprecision(10) << node->frequency << "\n";
    }
    else {
        // 内点，继续走
        travelTree(node->left, code + '0');
        travelTree(node->right, code + '1');
    }
}

// 释放节点并设置节点为空
void freeTree(Node *&node) {
    if (node != nullptr) {
        freeTree(node->left);
        freeTree(node->right);
        delete node;
        node = nullptr;
    }
}

int main() {
    // 输入的文件名
    std::string file_name;
    std::cout << "Please input file name:";
    // 获取一行
    std::getline(std::cin, file_name);
    // 打开文件流
    std::fstream file(file_name, std::ios_base::in);
    // 所有字符个数
    int sum = 0;
    // 字符
    char ch;
    // 获取频率时使用的map
    std::map<char, int>map;
    // 查数
    if (file.is_open()) {
        // 未到文件尾，继续读
        while (!file.eof()) {
            // 输入
            file >> ch;
            // 搜索一下在不在里面，其实map的话不在里面int会初始为0，不管也行
            //if (map.find(ch) != map.end()) {
                //++map[ch];
            //}
            //else {
            //    map[ch] = 1;
            //}
            ++map[ch];
            ++sum;
        }
        file.close();
    }
    // 打开错误了
    else {
        std::cerr << "Open file error.\n";
        exit(-1);
    }
    // 注意此迭代器可改变元素，建议使用const iter
    auto iter = map.cbegin();
    auto end = map.cend();
    // 优先队列，懒得自己维护红黑树（其实是因为我不会红黑树）
    std::priority_queue<Node>queue;
    // 注意map的底层是红黑树，在遍历时很可能时按照ASCII码的顺序打印的    
    while (iter != end) {
        // 将node的相关信息弄进去
        queue.push(Node((*iter).first, (*iter).second * 1.0 / sum));
        // 输出一下吧
        std::cout << "'" << (*iter).first << "': " << "times: " << (*iter).second << "\n";
        // 迭代器别忘了往下挪
        ++iter;
    }
    // 当剩余不止一个节点时
    while (queue.size() > 1) {
        // 此处使用指针，在最开始的时候会炸掉原来是因为自动分配的node在下一轮循环时会被释放掉
        // 我勒个去，取得的时局部变量的指针，当然会炸了，还是Java有GC好
        auto left = new Node(queue.top());
        queue.pop();
        auto right = new Node(queue.top());
        queue.pop();
        // 建一个新的节点，放进去
        Node node(left->frequency + right->frequency);
        // 左右指针赋值
        node.left = left;
        node.right = right;
        // 得到的节点再加进去
        queue.push(node);
    }
    // 最后一个节点，也就是HuffmanTree的根啦
    auto top = new Node(queue.top());
    // 弹出去，相当于清空了，我在想要不要把new的空间释放掉
    queue.pop();
    // 遍历时得到编码，没有存储的需要的话就直接打印吧
    travelTree(top, "");
    // 释放树
    freeTree(top);

    return 0;
}
