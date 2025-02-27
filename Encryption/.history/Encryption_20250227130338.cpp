#include "Encryption.h"
#include <memory>
#include "openssl/include/openssl/bio.h"
#include "openssl/include/openssl/evp.h"
#include "openssl/include/openssl/buffer.h"

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

std::string CommonFunctions::GetMK(const unsigned char* cInternalKey, const std::string& permutation) {
    std::array<unsigned char, 32> cInternalKeyNonShuffled{};
    std::string sInternalKeyShuffled(reinterpret_cast<const char*>(cInternalKey), 32);

    std::memset(cInternalKeyNonShuffled.data(), 0, cInternalKeyNonShuffled.size());

    int indexForsInternalKey = 0;
    for (char ch : permutation) {
        int value = ch - '0';
        value *= 4;

        for (int y = value; y < (value + 4); ++y) {
            cInternalKeyNonShuffled[y] = cInternalKey[indexForsInternalKey++];
        }
    }

    std::string base64sConverted = CommonFunctions::base64(cInternalKeyNonShuffled.data(), 32);
    return base64sConverted;
}