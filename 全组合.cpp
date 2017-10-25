#include <iostream>
#include <vector>
#include <algorithm>

void search_all(std::vector<int> &value, std::vector<int> &in, const int &index) {
    if (index == value.size()) {
        // do something
         for (int i = 0; i < value.size(); ++i) {
             if (in[i] == 1) {
                 std::cout << value[i] << " ";
             }
         }

        std::cout << "\n";
        return;
    }

    in[index] = 0;
    search_all(value, in, index + 1);
    in[index] = 1;
    search_all(value, in, index + 1);
}

int main(int argc, char *argv[]) {
    std::vector<int> v{1, 2, 3, 4, 5};
    std::vector<int> in(v.size(), 1);
    search_all(v, in, 0);
    return 0;
}
