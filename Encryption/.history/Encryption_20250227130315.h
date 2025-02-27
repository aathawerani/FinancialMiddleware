#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

class Encryption {
public:
    static std::string base64(const unsigned char* input, int length);
    static std::string GetMK(const unsigned char* cInternalKey, const std::string& permutation);
};

#endif // ENCRYPTION_H
