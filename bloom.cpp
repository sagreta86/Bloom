#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <vector>
#include <array>
#include <cstddef>

#include "smhasher/MurmurHash3.h"

//basic structure of a bloom filter object
struct BloomFilter {
    BloomFilter(size_t size, uint8_t numHashes);
    void add(const uint8_t* data, std::size_t len);
    bool possiblyContains(const uint8_t* data, std::size_t len) const;
private:
    uint8_t m_numHashes;
    std::vector<bool> m_bits;
};
//Bloom filter constructor
BloomFilter::BloomFilter(size_t size, uint8_t numHashes)
    : m_bits(size),
    m_numHashes(numHashes) {}
//Hash array created using the MurmurHash3 code
static std::array<uint64_t, 2> myhash(const uint8_t* data, std::size_t len)
{
    std::array<uint64_t, 2> hashValue{};
    MurmurHash3_x64_128(data, len, 0, hashValue.data());
    return hashValue;
}
//Hash array created using the MurmurHash3 code
inline size_t nthHash(int n,
    uint64_t hashA,
    uint64_t hashB,
    size_t filterSize) {
    return (hashA + n * hashB) % filterSize; // <- not sure if that is OK, perhaps it is.
}
//Adds an element to the array
void BloomFilter::add(const uint8_t* data, std::size_t len) {
    auto hashValues = myhash(data, len);
    for (int n = 0; n < m_numHashes; n++)
    {
        m_bits[nthHash(n, hashValues[0], hashValues[1], m_bits.size())] = true;
    }
}
//Returns true or false based on a probabilistic assesment of the array         using MurmurHash3
bool BloomFilter::possiblyContains(const uint8_t* data, std::size_t   len) const {
    auto hashValues = myhash(data, len);
    for (int n = 0; n < m_numHashes; n++)
    {
        if (!m_bits[nthHash(n, hashValues[0], hashValues[1], m_bits.size())])
        {
            return false;
        }
    }
    return true;
}
#endif

#include <functional>
#include <iostream>
#include <assert.h>

using namespace std;

int main()
{
    BloomFilter bf(1024 * 1024, 5);
    std::string s = "12345";
    bf.add((uint8_t*)s.c_str(), s.size());
    bool possiblyContains = bf.possiblyContains((uint8_t*)s.c_str(), s.size());
    cout << "possible: " << possiblyContains << endl;
    assert(possiblyContains);
}