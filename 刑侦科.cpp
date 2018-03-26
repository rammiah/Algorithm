#include <iostream>
#include <vector>
#include <algorithm>

char ans[11] = {0, 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'};

bool check_ans() {
    // 第一题不用考虑
    // 第二题
    if (ans[2] == 'A' && ans[5] != 'C') {
        return false;
    } else if (ans[2] == 'B' && ans[5] != 'D') {
        return false;
    } else if (ans[2] == 'C' && ans[5] != 'A') {
        return false;
    } else if (ans[2] == 'D' && ans[5] != 'B') {
        return false;
    }
    // 第三题
    if (ans[3] == 'A' && !(ans[3] != ans[6] && ans[2] == ans[6])) {
        return false;
    } else if (ans[3] == 'B' && !(ans[3] == ans[2] && ans[3] != ans[6])) {
        return false;
    } else if (ans[3] == 'C' && !(ans[3] == ans[6] && ans[2] != ans[6])) {
        return false;
    } else if (ans[3] == 'D' && !(ans[3] == ans[6] && ans[4] != ans[6])) {
        return false;
    }
    // 第四题
    if (ans[4] == 'A' && ans[1] != ans[5]) {
        return false;
    } else if (ans[4] == 'B' && ans[2] != ans[7]) {
        return false;
    } else if (ans[4] == 'C' && ans[1] != ans[9]) {
        return false;
    } else if (ans[4] == 'D' && ans[6] != ans[10]) {
        return false;
    }
    // 第五题
    if (ans[5] == 'A' && ans[5] != ans[8]) {
        return false;
    } else if (ans[5] == 'B' && ans[5] != ans[4]) {
        return false;
    } else if (ans[5] == 'C' && ans[5] != ans[9]) {
        return false;
    } else if (ans[5] == 'D' && ans[5] != ans[7]) {
        return false;
    }
    // 第六题
    if (ans[6] == 'A' && !(ans[2] == ans[8] && ans[4] == ans[8])) {
        return false;
    } else if (ans[6] == 'B' && !(ans[1] == ans[8] && ans[6] == ans[8])) {
        return false;
    } else if (ans[6] == 'C' && !(ans[3] == ans[8] && ans[10] == ans[8])) {
        return false;
    } else if (ans[6] == 'D' && !(ans[5] == ans[8] && ans[9] == ans[8])) {
        return false;
    }
    // 第七题
    int times[4] = {0,0,0,0};
    for (int i = 1; i <= 10; ++i) {
        ++times[ans[i] - 'A'];
    }
    int min = 11;
    char least = 'A';
    for (int i = 0; i < 4; ++i) {
        if (times[i] < min) {
            min = times[i];
            least = 'A' + i;
        }
    }
    if (ans[7] == 'A' && least != 'C') {
        return false;
    } else if (ans[7] == 'B' && least != 'B') {
        return false;
    } else if (ans[7] == 'C' && least != 'A') {
        return false;
    } else if (ans[7] == 'D' && least != 'D') {
        return false;
    }
    // 第八题
    // 不相邻这个666
    if (ans[8] == 'A' && std::abs(ans[1] - ans[7]) == 1) {
        return false;
    } else if (ans[8] == 'B' && std::abs(ans[1] - ans[5]) == 1) {
        return false;
    } else if (ans[8] == 'C' && std::abs(ans[1] - ans[2]) == 1) {
        return false;
    } else if (ans[8] == 'D' && std::abs(ans[1] - ans[10]) == 1) {
        return false;
    }
    // 第九题
    if (ans[9] == 'A' && ((ans[1] == ans[6]) == (ans[6] == ans[5]))) {
        return false;
    } else if (ans[9] == 'B' && ((ans[1] == ans[6]) == (ans[10] == ans[5]))) {
        return false;
    } else if (ans[9] == 'C' && ((ans[1] == ans[6]) == (ans[2] == ans[5]))) {
        return false;
    } else if (ans[9] == 'D' && ((ans[1] == ans[6]) == (ans[9] == ans[5]))) {
        return false;
    }
    // 第十题
    times[0] = times[1] = times[2] = times[3] = 0;
    int max = -1;
    min = 11;
    for (int i = 1; i <= 10; ++i) {
        ++times[ans[i] - 'A'];
    }
    for (int i = 0; i < 4; ++i) {
        min = std::min(min, times[i]);
        max = std::max(max, times[i]);
    }

    if (ans[10] == 'A' && max - min != 3) {
        return false;
    } else if (ans[10] == 'B' && max - min != 2) {
        return false;
    } else if (ans[10] == 'C' && max - min != 4) {
        return false;
    } else if (ans[10] == 'D' && max - min != 1) {
        return false;
    }

    return true;
}

bool dfs(int index) {
    if (index == 11) {
        return check_ans();
    }

    for (int i = 0; i < 4; ++i) {
        ans[index] = 'A' + i;
        if (dfs(index + 1)) {
            return true;
        }
    }

    return false;
}

int main() {
    dfs(1);
    std::cout << check_ans() << "\n";

    for (int i = 1; i <= 10; ++i) {
        std::cout << i << ": " << ans[i] << "\n";
    }

    return 0;
}