#include <iostream>
#include <fstream>
#include <vector>

bool isValid(const std::vector<std::vector<bool>> &data, const std::vector<int> &haveAdd, const int &toTest) {
    for (const auto &t : haveAdd) {
        if (!data[toTest][t]) {
            return false;
        }
    }
    return true;
}

void search(const std::vector<std::vector<bool>> &data, std::vector<int> &haveAdd,
            const int &from, const int &currentSize,  int &max, const int &size) {
    for (int i = from + 1; i < size; ++i) {
        if (isValid(data, haveAdd, i)) {
            haveAdd.push_back(i);
            max = max < currentSize + 1? currentSize : max;
            search(data, haveAdd, i, currentSize + 1, max, size);
            haveAdd.pop_back();
        }
    }
}

int main() {

//    std::ifstream in;
//    std::ofstream outfile;
//    // open file
//    in.open("test0.dat");
//    outfile.open("mydata.dat");
//
//    int vertex = 0, edge = 0;
//    // a char needed
//    char c;
//    // read the vertex count and edge count
//    in >> vertex >> edge;
//
//    // write
//    outfile << vertex << " " << edge << std::endl;
//    int from = 0, to = 0;
//    // read the real data
//    while (in >> c >> from >> to) {
//        outfile << from << " " << to << std::endl;
//    }
//
//    // remember to close file
//    in.close();
//    outfile.close();

    std::ifstream infile;
    infile.open("bak.dat");
    int vertexes = 0, edges = 0;
    infile >> vertexes >> edges;
    std::vector<std::vector<bool>> allData(static_cast<unsigned long>(vertexes + 1),
                                           std::vector<bool>(static_cast<unsigned long>(vertexes + 1), false));
    int from = 0, to = 0;

    for (int i = 0; i < edges; ++i) {
        infile >> from >> to;
        allData[from][to] = allData[to][from] = true;
    }
    infile.close();
    int max = 0;
    std::vector<int> haveAdd;
    search(allData, haveAdd, 0, 0, max, static_cast<const int &>(allData.size()));
    std::cout << max << std::endl;

    return 0;
}
