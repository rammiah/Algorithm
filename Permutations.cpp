nclude <iostream>
#include <vector>

template<typename T>
void swap(T &a, T &b) {
        T t = a;
        a = b;
        b = t;

}

void search(std::vector<int> &v, const int &index) {
    if (index == v.size() - 1) {
        for (const int &t : v) {
                        std::cout << t << " ";
                    
        }
                std::cout << std::endl;
            
    }

    for (int i = index; i < v.size(); ++i) {
                swap<int>(v[i], v[index]);
                search(v, index + 1);
                swap<int>(v[i], v[index]);
            
    }

}

int main() {
        std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};

        search(v, 0);

        return 0;

}

