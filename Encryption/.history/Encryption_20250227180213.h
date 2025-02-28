#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <span>
#include <vector>

class Encryption {
public:
    static std::string base64(const unsigned char* input, int length);
    static std::string GetMK(const unsigned char* cInternalKey, const std::string& permutation);
    static bool Compress(std::span<const uint8_t> srcBuffer, std::vector<uint8_t>& destBuffer);
    static bool Uncompress(std::span<const uint8_t> srcBuffer, std::vector<uint8_t>& destBuffer);
    static std::string HashValue(std::string sData);
    static std::string SHA1Hash(std::string_view binaryContent, std::string_view salt);    
};

#endif // ENCRYPTION_H
