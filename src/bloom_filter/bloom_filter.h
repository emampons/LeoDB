#ifndef LEODB_BLOOM_FILTER_H
#define LEODB_BLOOM_FILTER_H

#include <vector>
#include <string>
#include <math.h>
#include "murmurhash.h"

class BloomFilter {
public:
    BloomFilter();

    BloomFilter(int numElement_, int bitsPerElement_);

    int numElement;
    int bitsPerElement;

    void program(std::string key);

    bool query(std::string key);

    int getIndexNum();

    int getSize();

private:
    int numIndex;
    int size;
    std::vector<bool> bf_vec;

    void makeBloomFilter();

    void getIndex(std::string key, std::vector<int> *index);
};
