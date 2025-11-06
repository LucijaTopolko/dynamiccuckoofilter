#ifndef HASH_UTILS_H
#define HASH_UTILS_H

#include <cstdint>
#include <cstddef>

/**
 * MurmurHash3 32-bit verzija
 * @param key - pointer na podatke za hashiranje
 * @param len - duljina podataka u bajtovima
 * @param seed - seed vrijednost za hash funkciju
 * @return 32-bit hash vrijednost
 */
uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t seed);

/**
 * Hash funkcija za proizvoljan item
 * @param item - pointer na item
 * @param size - veličina itema u bajtovima
 * @param seed - seed vrijednost (default: 0)
 * @return 32-bit hash vrijednost
 */
uint32_t hash_item(const void* item, size_t size, uint32_t seed = 0);

/**
 * Generira fingerprint od itema
 * Fingerprint je kompaktna reprezentacija itema koja se sprema u cuckoo filter
 * @param item - pointer na item
 * @param size - veličina itema u bajtovima
 * @param fingerprint_bits - broj bitova za fingerprint (default: 16)
 * @return fingerprint vrijednost (nikad nije 0)
 */
uint32_t generate_fingerprint(const void* item, size_t size, uint8_t fingerprint_bits = 16);

/**
 * Izračunava primarnu bucket lokaciju za item
 * @param item - pointer na item
 * @param size - veličina itema u bajtovima
 * @param num_buckets - ukupan broj bucketa u filteru
 * @return indeks primarne bucket pozicije
 */
size_t get_primary_bucket(const void* item, size_t size, size_t num_buckets);

/**
 * Izračunava alternativnu bucket lokaciju koristeći primarnu poziciju i fingerprint
 * Koristi XOR operaciju: i2 = i1 XOR hash(fingerprint)
 * @param primary_bucket - primarna bucket pozicija
 * @param fingerprint - fingerprint itema
 * @param num_buckets - ukupan broj bucketa u filteru
 * @return indeks alternativne bucket pozicije
 */
size_t get_alternate_bucket(size_t primary_bucket, uint32_t fingerprint, size_t num_buckets);

/**
 * Izračunava alternativnu bucket lokaciju iz bilo koje trenutne pozicije
 * Funkcija je simetrična: ako je i2 = alt(i1, fp), onda je i1 = alt(i2, fp)
 * @param current_bucket - trenutna bucket pozicija
 * @param fingerprint - fingerprint itema
 * @param num_buckets - ukupan broj bucketa u filteru
 * @return indeks alternativne bucket pozicije
 */
size_t get_alternate_bucket_from_current(size_t current_bucket, uint32_t fingerprint, size_t num_buckets);

/**
 * Helper funkcija za hashiranje uint64_t vrijednosti
 * @param value - 64-bit cijeli broj
 * @return 32-bit hash vrijednost
 */
uint32_t hash_uint64(uint64_t value);

/**
 * Helper funkcija za hashiranje C stringa
 * @param str - null-terminated string
 * @return 32-bit hash vrijednost
 */
uint32_t hash_string(const char* str);

#endif // HASH_UTILS_H

