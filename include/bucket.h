#ifndef BUCKET_H
#define BUCKET_H

#include <vector>
#include <cstdint>
#include <cstddef>
#include <iostream>

class Bucket {
public:
    explicit Bucket(size_t capacity = 4);

    /*
     * ubacuje fingerprint u bucket
     * @param fp - fingerprint koji se ubacuje (ne smije biti 0)
     * @return true ako je ubacivanje uspjelo, false ako je bucket pun
     */
    bool insert(uint8_t fp);

    /**
     * uklanja fingerprint iz bucketa
     * @param fp - fingerprint koji se uklanja
     * @return true ako je fingerprint uspješno uklonjen
     */
    bool remove(uint8_t fp);

    /**
     * provjerava postoji li fingerprint u bucketu
     * @param fp - fingerprint koji se traži
     * @return true ako fingerprint postoji u bucketu
     */
    bool contains(uint8_t fp) const;

    /**
     * izbacuje nasumični fingerprint iz bucketa
     * @return izbačeni fingerprint, ili 0 ako je bucket prazan
     */
    uint8_t evict_random();

    /**
     * provjerava je li bucket pun
     * @return true ako je bucket pun
     */
    bool is_full() const;

    /**
     * ispisuje sadržaj bucketa na zadani izlazni stream
     * @param os - izlazni stream (default: std::cout)
     */
    void print(std::ostream &os = std::cout) const;

private:
    std::vector<uint8_t> slots; // 0 označava prazno mjesto
};

#endif // BUCKET_H
