#include "bucket.h"
#include <algorithm>
#include <random>
#include <iomanip>

Bucket::Bucket(size_t capacity)
    : slots(capacity, 0) // inicijalizacija
{}

bool Bucket::insert(uint8_t fp) {
    if (fp == 0) return false; // fingerprint ne moze biti 0
    for (auto &s : slots) {
        if (s == 0) {
            s = fp;
            return true;
        }
    }
    return false; // ako je bucket pun
}

bool Bucket::remove(uint8_t fp) {
    if (fp == 0){
        return false;
    }
    for (auto &s : slots) {
        if (s == fp) {
            s = 0;
            return true;
        }
    }
    return false; // ako bucket nije pronaden
}

bool Bucket::contains(uint8_t fp) const {
    if (fp == 0) return false; // fingerprint ne moze biti 0
    for (auto &s : slots) {
        if (s == fp) {
            return true; // fingerprint pronađen
        }
    }
    return false; // fingerprint nije u bucketu
}

uint8_t Bucket::evict_random() {
    // odredi sve ne-prazne slotove
    std::vector<size_t> non_empty;
    for (size_t i = 0; i < slots.size(); ++i) {
        if (slots[i] != 0) non_empty.push_back(i);
    }
    if (non_empty.empty()) return 0; // ako je bucket prazan

    static thread_local std::mt19937 gen((std::random_device())());
    std::uniform_int_distribution<size_t> dist(0, non_empty.size() - 1);
    size_t chosen = non_empty[dist(gen)];
    uint8_t val = slots[chosen];
    slots[chosen] = 0;
    return val;
}

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
        if (slots[i] == 0) os << " --";
        else {
            os << "0x" << std::hex << std::setw(2) << std::setfill('0')
               << (int)slots[i] << std::dec;
        }
    }
    os << " ]";
}
