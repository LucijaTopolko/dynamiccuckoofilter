#include <cstdint>
#include <cstring>
#include <functional>

/**
 * Algoritam radi u 3 faze:
 * 1. Obrada blokova od 4 B (glavni dio podataka)
 * 2. Obrada ostatka (1-3 B ako dužina nije dijeliva sa 4)
 * 3. Finalizacija (Avalanche efekt - mala promjena ulaza jako mijenja izlaz)
 */
uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t seed) {
    uint32_t h = seed;
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;
    
    // FAZA 1: Obrada blokova od 4 B
    const int nblocks = len / 4;
    const uint32_t* blocks = (const uint32_t*)(key);
    
    for (int i = 0; i < nblocks; i++) {
        uint32_t k1 = blocks[i];
        
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= c2;
        
        h ^= k1;
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }
    
    // FAZA 2: Obrada ostatka (1-3 B)
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
    
    // FAZA 3: Finalizacija (Avalanche efekat)
    h ^= len; 
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    
    return h;
}

uint32_t hash_item(const void* item, size_t size, uint32_t seed = 0) {
    return murmur3_32((const uint8_t*)item, size, seed);
}

 // Generira fingerprint od itema
uint32_t generate_fingerprint(const void* item, size_t size, uint8_t fingerprint_bits = 16) {
    uint32_t hash = murmur3_32((const uint8_t*)item, size, 0xdeadbeef);
    
    // Uzmi donji dio hash-a za fingerprint (maskiraj na željeni broj bitova)
    uint32_t fingerprint = hash & ((1ULL << fingerprint_bits) - 1);
    
    if (fingerprint == 0) {
        fingerprint = 1;
    }
    
    return fingerprint;
}

/**
 * Izračunava primarnu lokaciju bucketa za item
 * 
 * Koristi hash sa seed-om 0 i modulo operaciju da dobije indeks bucketa.
 */
size_t get_primary_bucket(const void* item, size_t size, size_t num_buckets) {
    uint32_t hash = hash_item(item, size, 0);
    return hash % num_buckets;
}

/**
 * Izračunava alternativnu lokaciju bucketa koristeći fingerprint
 * 
 * - i1 = hash(x) % num_buckets  (primarna pozicija)
 * - i2 = i1 XOR hash(fingerprint) % num_buckets  (alternativna pozicija)
 * 
 * Koristimo treći seed (0xcafebabe) da osiguramo nezavisnost od
 * hash-a za primarnu poziciju i fingerprint generiranja.
 */
size_t get_alternate_bucket(size_t primary_bucket, uint32_t fingerprint, size_t num_buckets) {
    uint32_t fp_hash = murmur3_32((const uint8_t*)&fingerprint, sizeof(fingerprint), 0xcafebabe);
    
    size_t alternate = (primary_bucket ^ fp_hash) % num_buckets;
    
    return alternate;
}

size_t get_alternate_bucket_from_current(size_t current_bucket, uint32_t fingerprint, size_t num_buckets) {
    return get_alternate_bucket(current_bucket, fingerprint, num_buckets);
}

uint32_t hash_uint64(uint64_t value) {
    return murmur3_32((const uint8_t*)&value, sizeof(value), 0);
}

uint32_t hash_string(const char* str) {
    return murmur3_32((const uint8_t*)str, strlen(str), 0);
}

