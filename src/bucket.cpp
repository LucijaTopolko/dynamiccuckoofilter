#include "bucket.h"
#include <algorithm>
#include <random>
#include <iomanip>

 // Konstruktor - kreira bucket sa zadatim kapacitetom
Bucket::Bucket(size_t capacity)
    : slots(capacity, 0)
{}

/**
 * Ubacuje fingerprint u bucket
 * 
 * Algoritam: Traži prvi prazan slot i ubacuje fingerprint tamo.
 */
bool Bucket::insert(uint8_t fp) {
    if (fp == 0) return false;
    
    for (auto &s : slots) {
        if (s == 0) {
            s = fp;
            return true;
        }
    }
    return false;
}

/**
 * Uklanja fingerprint iz bucketa
 * 
 * Algoritam: Traži fingerprint u bucketu i postavlja slot na 0 (prazan).
 */
bool Bucket::remove(uint8_t fp) {
    if (fp == 0) {
        return false;
    }
    
    for (auto &s : slots) {
        if (s == fp) {
            s = 0;
            return true;
        }
    }
    return false;
}

 // Provjerava sadrži li bucket zadani fingerprint
bool Bucket::contains(uint8_t fp) const {
    if (fp == 0) return false;
    
    for (auto &s : slots) {
        if (s == fp) {
            return true;
        }
    }
    return false;
}

/**
 * Izbacuje nasumični fingerprint iz bucketa
 * 
 * Algoritam:
 * 1. Pronađi sve ne-prazne slotove
 * 2. Nasumično izaberi jedan
 * 3. Sačuvaj vrijednost, postavi slot na 0, vrati vrijednost
 * 
 * @return izbačeni fingerprint ili 0 ako je bucket prazan
 */
uint8_t Bucket::evict_random() {
    std::vector<size_t> non_empty;
    for (size_t i = 0; i < slots.size(); ++i) {
        if (slots[i] != 0) {
            non_empty.push_back(i);
        }
    }
    
    if (non_empty.empty()) {
        return 0;
    }

    static thread_local std::mt19937 gen((std::random_device())());
    std::uniform_int_distribution<size_t> dist(0, non_empty.size() - 1);
    size_t chosen = non_empty[dist(gen)];
    
    uint8_t val = slots[chosen];
    slots[chosen] = 0;
    return val;
}

 // Proverava je li bucket pun
bool Bucket::is_full() const {
    for (auto &s : slots) {
        if (s == 0) {
            return false;
        }
    }
    return true;
}

void Bucket::print(std::ostream &os) const {
    os << "[";
    for (size_t i = 0; i < slots.size(); ++i) {
        if (i) os << " ";
        
        if (slots[i] == 0) {
            os << " --";
        } else {
            os << "0x" << std::hex << std::setw(2) << std::setfill('0')
               << (int)slots[i] << std::dec;
        }
    }
    os << " ]";
}
