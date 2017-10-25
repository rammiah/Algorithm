#include <iostream>
#include <vector>
#include <algorithm>

void reverse(std::vector<int> &v, const int &start) {
    int e = v.size() - 1;
    int s = start;
    if (s >= e) {
        return;
    }
    while (s < e) {
        std::swap(v[s], v[e]);
        ++s;
        --e;
    }
}

bool next(std::vector<int> &v){
    int e = v.size() - 1;
    while (e >= 1 && v[e] < v[e - 1]) --e;
    if (e == 0) {
        std::cout << "No next permutations!" << std::endl;
        return false;
    } 
    int last_bigger = v.size() - 1;
    while (v[last_bigger] < v[e - 1]) --last_bigger;
    std::swap(v[last_bigger], v[e - 1]);
    reverse(v, e);
    return true;
}

int main(int argc, char *argv[]) {
    std::vector<int> v{1, 2, 3, 4, 5};
    std::for_each(v.begin(), v.end(), [](int t){std::cout << t << "";});
    std::cout << "\n";
    while (next(v)) {
        std::for_each(v.begin(), v.end(), [](int t){std::cout << t << "";});
        std::cout << "\n";      
    }
    
    return 0;
}
