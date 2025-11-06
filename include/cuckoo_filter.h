#include <string>
#include <vector>
using namespace std;

#include "bucket.h"

class CuckooFilter {
public:
    CuckooFilter(size_t num_buckets, size_t bucket_size, uint8_t fingerprints);
    bool insert(const string &key);
    bool lookup(const string &key) const;
    bool remove(const string &key);
    size_t size() const;
    double load_factor() const;

private:
    bool insert_fingerprint(uint32_t i1, uint32_t i2, uint8_t fingerprint);
    vector<Bucket> table;
    size_t num_buckets;
    size_t bucket_size;
    uint8_t fingerprints;
    const int MAX_KICKS = 500;
};