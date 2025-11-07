#include "hash_utils.h"
#include "bucket.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

void print_hex(const string& label, uint32_t value, int width = 8) {
    cout << label << ": 0x" << hex << setw(width) << setfill('0') << value << dec << endl;
}

void test_basic_hashing() {
    cout << "=== TEST 1: Osnovno hashiranje ===" << endl << endl;
    
    // Test sa stringom
    const char* str = "cuckoo filter";
    uint32_t hash1 = hash_string(str);
    print_hex("Hash od '" + string(str) + "'", hash1);
    
    // Test sa integerom
    uint64_t number = 123456789;
    uint32_t hash2 = hash_uint64(number);
    print_hex("Hash od " + to_string(number), hash2);
    
    // Test sa custom strukturom
    struct Person {
        uint32_t id;
        char name[20];
    } person = {42, "Alice"};
    
    uint32_t hash3 = hash_item(&person, sizeof(person));
    print_hex("Hash od strukture", hash3);

    // Test sa DNA slijedom
    const char* dna = "ATCGATCGATCG";
    const char* dna2 = "ATCGATCGATCC";
    uint32_t hash4 = hash_string(dna);
    uint32_t hash5 = hash_string(dna2);
    print_hex("Hash od ATCGATCGATCG", hash4);
    print_hex("Hash od ATCGATCGATCC", hash5);
    
    cout << endl;
}

void test_fingerprint_generation() {
    cout << "=== TEST 2: Generiranje fingerprinta ===" << endl;
    
    vector<string> items = {"ATCGATCGATCG", "ATCGATCGATCC", "ACGT"};
    
    cout << "\nFingerprinti sa 16 bitova:" << endl;
    for (const auto& item : items) {
        uint32_t fp = generate_fingerprint(item.c_str(), item.length(), 16);
        cout << setw(12) << left << item << " -> " 
             << "0x" << hex << setw(4) << setfill('0') << fp << dec << endl;
    }
    
    cout << endl;
}

// Test 3: Bucket lokacije i alternativne pozicije
void test_bucket_locations() {
    cout << "=== TEST 3: Bucket lokacije ===" << endl << endl;
    
    const size_t NUM_BUCKETS = 128;
    
    vector<string> items = {"ATCGATCGATCG", "ATCGATCGATCC", "ACGT"};
    
    for (const auto& item : items) {
        uint32_t fp = generate_fingerprint(item.c_str(), item.length(), 16);
        
        size_t primary = get_primary_bucket(item.c_str(), item.length(), NUM_BUCKETS);
        
        size_t alternate = get_alternate_bucket(primary, fp, NUM_BUCKETS);
        
        size_t back_to_primary = get_alternate_bucket(alternate, fp, NUM_BUCKETS);
        
        cout << "Item: '" << item << "'" << endl;
        cout << "  Fingerprint: 0x" << hex << setw(4) << setfill('0') << fp << dec << endl;
        cout << "  Primarna pozicija:     " << setw(3) << primary << endl;
        cout << "  Alternativna pozicija: " << setw(3) << alternate << endl;
        cout << "  Povratak na primarnu:  " << setw(3) << back_to_primary;
        
        if (back_to_primary == primary) {
            cout << " ✓ (simetricno!)" << endl;
        } else {
            cout << " ✗ (GREŠKA!)" << endl;
        }
        cout << endl;
    }
}

int main() {
    cout << "╔════════════════════════════════════════════════════╗" << endl;
    cout << endl;
    
    test_basic_hashing();
    test_fingerprint_generation();
    test_bucket_locations();
    
    cout << "╚════════════════════════════════════════════════════╝" << endl;
    
    return 0;
}

