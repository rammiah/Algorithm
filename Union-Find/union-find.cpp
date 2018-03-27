#include <iostream>

struct Set{
    Set *parent = nullptr;
    int rank;
};

void make_set(Set *set) {
    set->parent = set;
    set->rank = 0;
}

Set *find_set(Set *set) {
    if (set != set->parent) {
        set->parent = find_set(set->parent);
    }
    return set->parent;
}

void link(Set *x, Set *y) {
    if (x->rank > y->rank) {
        y->parent = x;
    }else {
        x->parent = y;
        if (x->rank == y->rank) {
            ++y->rank;
        }
    }
}

void _union(Set *x, Set *y) {
    link(find_set(x), find_set(y));
}

int main() {
    Set set[16];
    for (int i = 0; i < 16; ++i) {
        make_set(&set[i]);
//        set[i].value = i;
    }
    // 0 2 4 6, 8 10, 12 14
    // 1, 3, 5, 7, 9, 11, 13, 15
    for (int i = 0; i < 13; i += 4) {
        _union(&set[i], &set[i + 2]);
    }
    _union(&set[0], &set[4]);

    // std::cout << find_set(&set[0])->parent << "\n";
    // std::cout << find_set(&set[2])->parent << "\n";
    // std::cout << find_set(&set[4])->parent << "\n";

    // std::cout << find_set(&set[7])->parent << "\n";
    // std::cout << find_set(&set[8])->parent << "\n";
    // std::cout << find_set(&set[1])->parent << "\n";
    // 计算集合个数
    // 应该是11个
    int cnt = 0;
    for (int i = 0; i < 16; ++i) {
        if (set[i].parent == &set[i]) {
            ++cnt;
        }
    }

    std::cout << "all is " << cnt << " sets\n";

    return 0;
}