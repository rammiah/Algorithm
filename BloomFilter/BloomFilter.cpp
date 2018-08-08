//
// Created by yaning on 6/20/18.
//

#include "BloomFilter.h"

void BloomFilter::add(const std::string &value) {
    for (auto &func : funcs) {
        bits.set(func.hash(value), true);
    }
}

bool BloomFilter::contains(const std::string &value) {
    for (auto &func : funcs) {
        if (!bits[func.hash(value)]) {
            return false;
        }
    }
    return true;
}

BloomFilter::BloomFilter() {
    int size = sizeof(seeds) / sizeof(int);
    for (int i = 0; i < size; ++i) {
        funcs.push_back(SimpleHash(seeds[i], DEFAULT_SIZE));
    }
}
