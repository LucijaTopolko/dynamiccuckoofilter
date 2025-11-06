#include <cstdint>
#include <vector>
using namespace std;

class Bucket {
public:
    Bucket(size_t capacity = 4);
    bool insert(uint8_t fingerprint);
    bool remove(uint8_t fingerprint);
    bool contains(uint8_t fingerprint) const;
    uint8_t relocate_random();
    bool is_full() const;

private:
    vector<uint8_t> slots;
};