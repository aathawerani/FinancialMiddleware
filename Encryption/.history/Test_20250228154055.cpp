#include "Encryption.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "Test: " << std::endl;

    const unsigned char data[] = "Hello, C++20!";
    std::string encoded = Encryption::base64(data, sizeof(data) - 1);    
    std::cout << "Base64 Encoded: " << encoded << std::endl;

    unsigned char cInternalKey[32] = {
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
        0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
        0x59, 0x5A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66
    };
    std::string permutation = "012345"; // Example permutation
    std::string result = Encryption::GetMK(cInternalKey, permutation);
    std::cout << "Base64 Encoded Key: " << result << std::endl;

    std::vector<uint8_t> inputData = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    std::vector<uint8_t> compressedData;
    std::vector<uint8_t> decompressedData(inputData.size() * 2);  // Allocate a large buffer for decompression

    // Compress
    if (Encryption::Compress(inputData, compressedData)) {
        std::cout << "Compression successful! Compressed size: " << compressedData.size() << " bytes\n";
    } else {
        std::cerr << "Compression failed!\n";
    }

    // Uncompress
    if (Encryption::Uncompress(compressedData, decompressedData)) {
        std::cout << "Decompression successful! Data: ";
        for (char c : decompressedData) {
            std::cout << c;
        }
        std::cout << '\n';
    } else {
        std::cerr << "Decompression failed!\n";
    }

    std::string data1 = "Hello, Secure Hash!";
    std::string salt = "randomSalt123";
    std::string sha512Hash = Encryption::HashValue(data1);
    std::cout << "SHA-512 Hash: " << sha512Hash << std::endl;

    std::vector<unsigned char> sampleData = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0xC2, 0xA9, 0x21, 0x00};
    const char* binfilename = "example.bin";  // Change this to your actual file
    CFileIO binfile(binfilename, std::ios::binary  | std::ios::out);
    binfile.WriteLine(sampleData);
    binfile.flush();
    binfile.close();
    std::cout << "Binary file '" << binfilename << "' generated successfully!\n";

    std::string sha1Hash = Encryption::SHA1Hash("example.bin", salt);
    std::cout << "SHA-1 Hash with Salt: " << sha1Hash << std::endl;

    return 0;
}
