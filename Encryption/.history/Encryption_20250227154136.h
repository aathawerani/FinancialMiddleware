#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <span>
#include <vector>
#include "openssl/sha.h"

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

class Encryption {
public:
    static std::string base64(const unsigned char* input, int length);
    static std::string GetMK(const unsigned char* cInternalKey, const std::string& permutation);
    static bool Compress(std::span<const uint8_t> srcBuffer, std::vector<uint8_t>& destBuffer);
    static bool Uncompress(std::span<const uint8_t> srcBuffer, std::vector<uint8_t>& destBuffer);
};

#endif // ENCRYPTION_H
