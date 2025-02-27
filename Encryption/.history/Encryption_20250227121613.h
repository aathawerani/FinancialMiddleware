#ifndef ENCRYPTION_H
#define ENCRYPTION_H


class Encryption {
public:
    static std::string base64(const unsigned char* input, int length);
};

#endif // ENCRYPTION_H
