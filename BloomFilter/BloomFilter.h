//
// Created by yaning on 6/20/18.
//

#ifndef FILTER_BLOOMFILTER_H
#define FILTER_BLOOMFILTER_H

#include <vector>
#include <bitset>

#include "SimpleHash.h"

class BloomFilter {
private:
    static const int DEFAULT_SIZE = 2 << 24;
    const int seeds[6] = {11, 13, 29, 37, 39, 41};
    std::bitset<DEFAULT_SIZE> bits;

    std::vector<SimpleHash> funcs;
public:
    BloomFilter();

    void add(const std::string &value);
    bool contains(const std::string &value);
};


#endif //FILTER_BLOOMFILTER_H
