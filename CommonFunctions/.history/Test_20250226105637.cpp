#include "CommonFunctions.h"
#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <atomic>
#include <thread>

int main() {
    try {
        // Test BinToHex
        const std::vector<unsigned char> binData = { 0x48, 0x65, 0x6C, 0x6C, 0x6F }; // "Hello"
        std::string hexStr = CommonFunctions::BinToHex(binData);
        std::cout << "BinToHex: " << hexStr << std::endl;

        // Test HexToBin
        std::string binStr = CommonFunctions::HexToBin(hexStr);
        std::cout << "HexToBin: " << binStr << std::endl;

        // Test StringSplit
        std::string_view str = "Hello,World,This,Is,A,Test";
        std::vector<std::string_view> splitStr = CommonFunctions::StringSplit(str, ",");

        std::cout << "StringSplit: ";
        for (const auto& s : splitStr) {
            std::cout << "[" << s << "] ";
        }
        std::cout << std::endl;

        // Test strMatch
        bool match = CommonFunctions::strMatch("Hello", "H*o");
        std::cout << "strMatch: " << (match ? "true" : "false") << std::endl;

        // Test parseKeyValueString
        std::vector<CommonFunctions::StructKeyValue> keyValueArray;
        CommonFunctions::parseKeyValueString("key1=value1;key2=value2", keyValueArray);
        for (const auto& kv : keyValueArray) {
            std::cout << "Key: " << kv.key << " Value: " << kv.value << std::endl;
        }

        // Test tokenize
        auto tokens = CommonFunctions::tokenize("apple,banana,grape", ",");
        std::cout << "tokenize: ";
        for (const auto& token : tokens) {
            std::cout << "[" << token << "] ";
        }
        std::cout << std::endl;

        // Test timeStamp
        SYSTEMTIME systemTime;
        GetSystemTime(&systemTime);
        std::string timeStr;
        CommonFunctions::timeStamp(timeStr, systemTime);
        std::cout << "timeStamp: " << timeStr << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::string ebcdicData = "\xC1\xC2\xC3\xC4";  // Example EBCDIC characters

    std::cout << "Original EBCDIC Data: ";
    for (unsigned char ch : ebcdicData) {
        std::cout << std::hex << static_cast<int>(ch) << " ";
    }
    std::cout << std::endl;

    // Convert to ASCII
    std::string asciiData = CommonFunctions::ebcdicToAscii(ebcdicData);
    std::cout << "Converted ASCII Data: " << asciiData << std::endl;

    // Convert back to EBCDIC
    std::string reconvertedEBCDIC = CommonFunctions::asciiToEbcdic(asciiData);
    std::cout << "Reconverted EBCDIC Data: ";
    for (unsigned char ch : reconvertedEBCDIC) {
        std::cout << std::hex << static_cast<int>(ch) << " ";
    }
    std::cout << std::endl;

    return 0;
}
