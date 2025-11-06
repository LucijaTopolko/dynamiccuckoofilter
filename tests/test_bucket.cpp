#include "bucket.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <cassert>

using namespace std;

void test_insert_remove_contains() {
    Bucket b(4);
    assert(!b.is_full());
    assert(!b.contains(1));

    // insert some values
    assert(b.insert(5));
    assert(b.insert(10));
    assert(b.insert(20));
    assert(b.insert(30));
    assert(b.is_full());
    // bucket is full, next insert should fail
    assert(!b.insert(40));

    // remove one and re-insert
    assert(b.remove(20));
    assert(!b.contains(20));
    assert(b.insert(40));
    assert(b.contains(40));

    // removing non-existing returns false
    assert(!b.remove(99));
}

void test_evict_random() {
    Bucket b(4);
    b.insert(1);
    b.insert(2);
    b.insert(3);

    // capture current values
    vector<int> before;
    for (uint8_t v : {1u,2u,3u}) if (b.contains(v)) before.push_back(v);
    int count_before = (int)before.size();
    assert(count_before >= 1);

    uint8_t ev = b.evict_random();
    // evicted value must be non-zero and must have been present before
    assert(ev != 0);
    bool was_present = false;
    for (int x : before) if ((uint8_t)x == ev) was_present = true;
    assert(was_present);

    // count of non-zero slots decreased by 1
    int count_after = 0;
    for (int v = 1; v <= 3; ++v) if (b.contains((uint8_t)v)) ++count_after;
    assert(count_after == count_before - 1);
}

void test_print_format() {
    Bucket b(2);
    b.insert(0x1); // 0x01
    // one slot empty
    std::ostringstream ss;
    b.print(ss);
    std::string out = ss.str();
    // should show brackets, a hex value and a --
    assert(out.find("[") != std::string::npos);
    assert(out.find("]") != std::string::npos);
    assert(out.find("0x01") != std::string::npos || out.find("0x1") != std::string::npos);
    assert(out.find("--") != std::string::npos);
}

int main() {
    cout << "Running Bucket tests..." << endl;
    test_insert_remove_contains();
    cout << "  insert/remove/contains: OK" << endl;
    test_evict_random();
    cout << "  evict_random: OK" << endl;
    test_print_format();
    cout << "  print format: OK" << endl;
    cout << "All Bucket tests passed." << endl;
    return 0;
}