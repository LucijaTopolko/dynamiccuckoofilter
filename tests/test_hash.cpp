#include "hash_utils.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Pomocna funkcija za ispis u hex formatu
void print_hex(const string& label, uint32_t value, int width = 8) {
    cout << label << ": 0x" << hex << setw(width) << setfill('0') << value << dec << endl;
}

// Test 1: Osnovno hashiranje različitih tipova podataka
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
    print_hex("Hash od Person strukture", hash3);
    
    cout << endl;
}

// Test 2: Generiranje fingerprinta
void test_fingerprint_generation() {
    cout << "=== TEST 2: Generiranje fingerprinta ===" << endl << endl;
    
    vector<string> items = {"apple", "banana", "cherry", "date", "elderberry"};
    
    cout << "Fingerprinti sa 8 bitova:" << endl;
    for (const auto& item : items) {
        uint32_t fp = generate_fingerprint(item.c_str(), item.length(), 8);
        cout << setw(12) << left << item << " -> " 
             << "0x" << hex << setw(2) << setfill('0') << fp << dec << endl;
    }
    
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
    
    const size_t NUM_BUCKETS = 128;  // Primjer: filter sa 128 bucketa
    
    vector<string> items = {"test1", "test2", "test3"};
    
    for (const auto& item : items) {
        // Generiraj fingerprint
        uint32_t fp = generate_fingerprint(item.c_str(), item.length(), 16);
        
        // Dobij primarnu bucket lokaciju
        size_t primary = get_primary_bucket(item.c_str(), item.length(), NUM_BUCKETS);
        
        // Dobij alternativnu bucket lokaciju
        size_t alternate = get_alternate_bucket(primary, fp, NUM_BUCKETS);
        
        // Provjeri simetričnost: alternativa od alternative je primarna
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

// Test 4: Kolizije fingerprinta
void test_fingerprint_collisions() {
    cout << "=== TEST 4: Kolizije fingerprinta ===" << endl << endl;
    
    const int NUM_ITEMS = 1000;
    const uint8_t FP_BITS = 8;  // Sa 8 bitova očekujemo više kolizija
    
    vector<uint32_t> fingerprints;
    int collisions = 0;
    
    // Generiraj fingerprinte za mnogo itema
    for (int i = 0; i < NUM_ITEMS; i++) {
        string item = "item_" + to_string(i);
        uint32_t fp = generate_fingerprint(item.c_str(), item.length(), FP_BITS);
        
        // Provjeri koliziju
        for (uint32_t existing_fp : fingerprints) {
            if (existing_fp == fp) {
                collisions++;
                break;
            }
        }
        
        fingerprints.push_back(fp);
    }
    
    int max_possible = (1 << FP_BITS);
    double collision_rate = (double)collisions / NUM_ITEMS * 100.0;
    
    cout << "Broj itema: " << NUM_ITEMS << endl;
    cout << "Fingerprint bitova: " << (int)FP_BITS << " (" << max_possible << " mogućih vrijednosti)" << endl;
    cout << "Broj kolizija: " << collisions << endl;
    cout << "Stopa kolizija: " << fixed << setprecision(2) << collision_rate << "%" << endl;
    cout << endl;
}

// Test 5: Distribuacija hash vrijednosti po bucketima
void test_distribution() {
    cout << "=== TEST 5: Distribucija po bucketima ===" << endl << endl;
    
    const size_t NUM_BUCKETS = 16;
    const int NUM_ITEMS = 1000;
    
    vector<int> bucket_counts(NUM_BUCKETS, 0);
    
    // Generiraj mnogo itema i broji koliko pada u svaki bucket
    for (int i = 0; i < NUM_ITEMS; i++) {
        string item = "element_" + to_string(i);
        size_t bucket = get_primary_bucket(item.c_str(), item.length(), NUM_BUCKETS);
        bucket_counts[bucket]++;
    }
    
    cout << "Distribucija " << NUM_ITEMS << " itema po " << NUM_BUCKETS << " bucketa:" << endl << endl;
    
    int max_count = *max_element(bucket_counts.begin(), bucket_counts.end());
    
    for (size_t i = 0; i < NUM_BUCKETS; i++) {
        cout << "Bucket " << setw(2) << i << ": ";
        cout << setw(4) << bucket_counts[i] << " |";
        
        // Vizualni prikaz
        int bar_length = (bucket_counts[i] * 50) / max_count;
        for (int j = 0; j < bar_length; j++) {
            cout << "█";
        }
        cout << endl;
    }
    
    // Izračunaj prosječno i std. devijaciju
    double avg = (double)NUM_ITEMS / NUM_BUCKETS;
    double variance = 0;
    for (int count : bucket_counts) {
        variance += (count - avg) * (count - avg);
    }
    variance /= NUM_BUCKETS;
    double stddev = sqrt(variance);
    
    cout << "\nProsječno po bucketu: " << fixed << setprecision(2) << avg << endl;
    cout << "Standardna devijacija: " << stddev << endl;
    cout << endl;
}

int main() {
    cout << "╔════════════════════════════════════════════════════╗" << endl;
    cout << "║     TEST PROGRAMA ZA CUCKOO FILTER HASH FUNKCIJE   ║" << endl;
    cout << "╚════════════════════════════════════════════════════╝" << endl;
    cout << endl;
    
    test_basic_hashing();
    test_fingerprint_generation();
    test_bucket_locations();
    test_fingerprint_collisions();
    test_distribution();
    
    cout << "╔════════════════════════════════════════════════════╗" << endl;
    cout << "║                  SVE TESTOVI GOTOVI                ║" << endl;
    cout << "╚════════════════════════════════════════════════════╝" << endl;
    
    return 0;
}

