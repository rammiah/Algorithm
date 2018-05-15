#include <string>
#include <vector>
// using std::string;
// using std::vector;
class Trie {
private:
    static const int Radix = 256;
    struct Node {
        Node *next[Radix];
        bool is_word = false;
        Node() {
            for (auto &ptr : next) {
                ptr = nullptr;
            }
        }
    };

    Node *root = new Node();

    void get_starts_with(Node *node, std::vector<std::string> &result, std::string str) {
        if (node == nullptr) {
            return;
        }
        if (node->is_word) {
            result.push_back(str);
        }
        std::string s = "a";
        for (int i = 0; i < Radix; ++i) {
            s[0] = i;
            get_starts_with(node->next[i], result, str + s);
        }
    }

public:
    void insert(const std::string &str) {
        // 插入单词的操作
        Node *node = root;
        for (const auto &ch : str) {
            if (node->next[ch] == nullptr) {
                node->next[ch] = new Node();
            }
            node = node->next[ch];
        }
        node->is_word = true;
    }

    bool starts_with(const std::string &str) {
        // 计算是否有字符串以str开头
        Node *node = root;
        for (const auto &ch : str) {
            if (node->next[ch] == nullptr) {
                return false;
            }
            node = node->next[ch];
        }
        return true;
    }

    bool contain(const std::string &str) {
        Node *node = root;
        for (const auto &ch : str) {
            if (node->next[ch] == nullptr) {
                return false;
            }
            node = node->next[ch];
        }

        return node->is_word;
    }

    std::vector<std::string> get_starts_with(const std::string &str) {
        std::vector<std::string> result;
        Node *node = root;
        for (const auto &ch : str) {
            if (node->next[ch] == nullptr) {
                return result;
            }
            node = node->next[ch];
        }
        get_starts_with(node, result, str);
        return result;
    }
};