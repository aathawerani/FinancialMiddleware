#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <memory>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

class Encryption {
public:
    static std::string base64(const unsigned char* input, int length);
};

#endif // ENCRYPTION_H
