#include <cstdint>
#include <cstring>
#include <functional>

uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t seed) {
    uint32_t h = seed;
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;
    
    const int nblocks = len / 4;
    const uint32_t* blocks = (const uint32_t*)(key);
    
    // Body
    for (int i = 0; i < nblocks; i++) {
        uint32_t k1 = blocks[i];
        
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> 17);  // ROTL32(k1, 15)
        k1 *= c2;
        
        h ^= k1;
        h = (h << 13) | (h >> 19);     // ROTL32(h, 13)
        h = h * 5 + 0xe6546b64;
    }
    
    // Tail
    const uint8_t* tail = (const uint8_t*)(key + nblocks * 4);
    uint32_t k1 = 0;
    
    switch (len & 3) {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
                k1 *= c1;
                k1 = (k1 << 15) | (k1 >> 17);
                k1 *= c2;
                h ^= k1;
    }
    
    // Finalization
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    
    return h;
}

// Hash funkcija za item
uint32_t hash_item(const void* item, size_t size, uint32_t seed = 0) {
    return murmur3_32((const uint8_t*)item, size, seed);
}

// Generira fingerprint od itema
// Fingerprint je kompaktna reprezentacija itema (npr. 8, 16 ili 32 bita)
uint32_t generate_fingerprint(const void* item, size_t size, uint8_t fingerprint_bits = 16) {
    // Koristi različit seed za fingerprint nego za hash
    uint32_t hash = murmur3_32((const uint8_t*)item, size, 0xdeadbeef);
    
    // Uzmi donji dio hash-a za fingerprint
    uint32_t fingerprint = hash & ((1ULL << fingerprint_bits) - 1);
    
    // Osiguraj da fingerprint nikad nije 0 (bitno za cuckoo filter)
    if (fingerprint == 0) {
        fingerprint = 1;
    }
    
    return fingerprint;
}

// Izračunaj primarnu lokaciju bucketa
size_t get_primary_bucket(const void* item, size_t size, size_t num_buckets) {
    uint32_t hash = hash_item(item, size, 0);
    return hash % num_buckets;
}

// Izračunaj alternativnu lokaciju bucketa koristeći fingerprint
// Ovo je ključna osobina cuckoo filtera: i1 = hash(x), i2 = i1 XOR hash(fingerprint)
size_t get_alternate_bucket(size_t primary_bucket, uint32_t fingerprint, size_t num_buckets) {
    // Hashiraj fingerprint
    uint32_t fp_hash = murmur3_32((const uint8_t*)&fingerprint, sizeof(fingerprint), 0xcafebabe);
    
    // XOR sa primarnom lokacijom
    size_t alternate = (primary_bucket ^ fp_hash) % num_buckets;
    
    return alternate;
}

// Preopterećena verzija koja izračunava alternativnu poziciju iz bilo koje trenutne pozicije
// Funkcija je simetrična: ako je i2 = alt(i1, fp), onda je i1 = alt(i2, fp)
size_t get_alternate_bucket_from_current(size_t current_bucket, uint32_t fingerprint, size_t num_buckets) {
    return get_alternate_bucket(current_bucket, fingerprint, num_buckets);
}

// Helper funkcija za hashiranje uint64_t vrijednosti
uint32_t hash_uint64(uint64_t value) {
    return murmur3_32((const uint8_t*)&value, sizeof(value), 0);
}

// Helper funkcija za hashiranje stringa
uint32_t hash_string(const char* str) {
    return murmur3_32((const uint8_t*)str, strlen(str), 0);
}

