//
// Created by yaning on 6/20/18.
//

#ifndef FILTER_SIMPLEHASH_H
#define FILTER_SIMPLEHASH_H

#include "Hash.h"

class SimpleHash : public Hash {
private:
    int seed;
    int cap;
public:
    SimpleHash(int seed, int cap);

public:
    unsigned int hash(const std::string &value);
};


#endif //FILTER_SIMPLEHASH_H
