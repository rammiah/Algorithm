//
// Created by yaning on 6/20/18.
//

#include "SimpleHash.h"

unsigned int SimpleHash::hash(const std::string &value) {
    unsigned int result = 0;

    for (auto &ch : value) {
        result = result * seed + ch;
    }

    return (cap - 1) & result;
}

SimpleHash::SimpleHash(int seed, int cap) : seed(seed), cap(cap) {}
