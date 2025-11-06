#pragma once
#include <memory>
#include <string>
#include <vector>
using namespace std;

#include "cuckoo_filter.h"


class DynamicCuckooFilter {
public:
    DynamicCuckooFilter(size_t init_buckets, size_t bucket_size, uint8_t fingerprints);
    bool insert(const string &key);
    bool lookup(const string &key) const;
    bool remove(const string &key);
    void expand();
    size_t total_size() const;

private:
    vector<unique_ptr<CuckooFilter>> filters; // TODO: include za CuckooFilter kad bude implementiran
    size_t bucket_size;
    uint8_t fingerprints;
};