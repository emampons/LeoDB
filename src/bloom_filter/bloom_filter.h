#ifndef LEODB_BLOOM_FILTER_H
#define LEODB_BLOOM_FILTER_H

#include <vector>
#include <string>
#include <data/key.h>
#include <math.h>
#include <bloom_filter/murmurhash.cpp>


class BloomFilter {
public:
    BloomFilter();
    BloomFilter(int numElement_, int bitsPerElement_);
    int numElement;
    int bitsPerElement;
    void program(int key);
    bool query(int key);
    int getIndexNum();
    int getSize();

private:
    int numIndex;
    int size;
    std::vector<bool> bf_vec;
    void makeBloomFilter();
    void getIndex(std::string key, std::vector<int> *index);
};

#endif