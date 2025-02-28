#include "Encryption.h"

#include <memory>
#include <cstring>   // For std::memset
#include <vector>
#include <array> 
#include <stdexcept>

#include "cryptopp/filters.h"
#include "cryptopp/hex.h"
#include "cryptopp/sha.h"

#include "openssl/include/openssl/sha.h"
#include "openssl/include/openssl/bio.h"
#include "openssl/include/openssl/evp.h"
#include "openssl/include/openssl/buffer.h"

#include "zlib/zlib.h"

#include "../CommonFunctions/CommonFunctions.h"
#include "../FileIO/FileIO.h"


// Automatically link OpenSSL libraries
#pragma comment(lib, "openssl/lib/libssl.lib")
#pragma comment(lib, "openssl/lib/libcrypto.lib")

std::string Encryption::base64(const unsigned char* input, int length) {
    BIO* bmem = nullptr;
    BIO* b64 = nullptr;
    BUF_MEM* bptr = nullptr;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    std::string encoded(bptr->data, bptr->length - 1);
    
    BIO_free_all(b64);

    return encoded;
}

std::string Encryption::GetMK(const unsigned char* cInternalKey, const std::string& permutation) {
    std::array<unsigned char, 32> cInternalKeyNonShuffled{};
    std::string sInternalKeyShuffled(reinterpret_cast<const char*>(cInternalKey), 32);

    std::memset(cInternalKeyNonShuffled.data(), 0, cInternalKeyNonShuffled.size());

    int indexForsInternalKey = 0;
    for (char ch : permutation) {
        int value = ch - '0';
        value *= 4;

        if (value + 4 > 32) {  
            continue;
        }

        for (int y = value; y < (value + 4); ++y) {
            cInternalKeyNonShuffled[y] = cInternalKey[indexForsInternalKey++];
        }
    }

    return Encryption::base64(cInternalKeyNonShuffled.data(), 32);
}

bool Encryption::Compress(std::span<const uint8_t> srcBuffer, std::vector<uint8_t>& destBuffer) {
    uLongf destLength = compressBound(static_cast<uLong>(srcBuffer.size()));

    destBuffer.resize(destLength);  // Allocate enough space for compression

    int result = compress2(destBuffer.data(), &destLength, srcBuffer.data(), static_cast<uLong>(srcBuffer.size()), Z_BEST_COMPRESSION);
    if (result != Z_OK) {
        return false;  // Compression failed
    }

    destBuffer.resize(destLength);  // Shrink to actual compressed size
    return true;
}

bool Encryption::Uncompress(std::span<const uint8_t> srcBuffer, std::vector<uint8_t>& destBuffer) {
    uLongf destLength = destBuffer.size();  // Expected decompressed size

    int result = uncompress(destBuffer.data(), &destLength, srcBuffer.data(), static_cast<uLong>(srcBuffer.size()));
    if (result != Z_OK) {
        return false;  // Decompression failed
    }

    destBuffer.resize(destLength);  // Shrink to actual decompressed size
    return true;
}

std::string Encryption::HashValue(std::string_view sData) {
    CryptoPP::SHA512 hasher;
    std::vector<uint8_t> hash(CryptoPP::SHA512::DIGESTSIZE);
    hasher.CalculateDigest(hash.data(), reinterpret_cast<const uint8_t*>(sData.data()), sData.size());
    return BinToHex(hash.data(), hash.size());
}

std::string Encryption::SHA1Hash(std::string binaryContent, std::string_view salt) {
    CFileIO binfile(binaryContent, std::ios::binary  | std::ios::in);
    std::vector<uint8_t> fileData = binfile.ReadBinary();
    fileData.insert(fileData.end(), salt.begin(), salt.end());  // Append salt

    uint8_t md[SHA_DIGEST_LENGTH];
    SHA1(fileData.data(), fileData.size(), md);

    return BinToHex(md, SHA_DIGEST_LENGTH);
}