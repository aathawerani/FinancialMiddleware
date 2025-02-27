#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <memory>

// Use local OpenSSL includes
#include "OpenSSL/include/bio.h"
#include "OpenSSL/include/evp.h"
#include "OpenSSL/include/buffer.h"

// Automatically link OpenSSL libraries
#pragma comment(lib, "third_party/openssl/lib/libssl.lib")
#pragma comment(lib, "third_party/openssl/lib/libcrypto.lib")

class Encryption {
public:
    static std::string base64(const unsigned char* input, int length);
};

#endif // ENCRYPTION_H
