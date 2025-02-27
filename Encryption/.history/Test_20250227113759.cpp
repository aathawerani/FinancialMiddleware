#include "Encryption.h"
#include <iostream>

int main() {
    const unsigned char data[] = "Hello, C++20!";
    std::string encoded = Encryption::base64(data, sizeof(data) - 1);
    
    std::cout << "Base64 Encoded: " << encoded << std::endl;
    return 0;
}
