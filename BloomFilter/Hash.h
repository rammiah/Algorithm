//
// Created by yaning on 6/20/18.
//

#ifndef FILTER_HASH_H
#define FILTER_HASH_H

#include <string>

class Hash {
    virtual unsigned int hash(const std::string &value)= 0;
};


#endif //FILTER_HASH_H
