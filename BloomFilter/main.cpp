#include <iostream>
#include "BloomFilter.h"


int main() {
//    std::cout << "Hello, World!" << std::endl;
    BloomFilter filter;
    std::string value = "Hello world.";
    std::cout << filter.contains(value) << "\n";
    filter.add(value);
    std::cout << filter.contains(value) << "\n";
    return 0;
}