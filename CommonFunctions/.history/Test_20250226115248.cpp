#include "CommonFunctions.h"
#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <atomic>
#include <thread>
#include <io.h>
#include <fcntl.h>

int main() {
    try {
        // Example: Convert Binary to Hex
        std::vector<unsigned char> binaryData = {0x12, 0xAB, 0xCD, 0xEF};
        std::string hexStr = CommonFunctions::BinToHex(binaryData);
        std::cout << "Binary to Hex: " << hexStr << std::endl;

        // Example: Convert Hex to Binary
        auto binDataOpt = CommonFunctions::HexToBin(hexStr);
        if (binDataOpt) {
            std::vector<unsigned char> binData(binDataOpt->begin(), binDataOpt->end());
            std::cout << "Hex to Binary: ";
            for (unsigned char byte : binData) {
                std::cout << std::hex << static_cast<int>(byte) << " ";
            }
            std::cout << std::endl;
        }
        
        // Example: String Split
        std::vector<std::string_view> tokens = CommonFunctions::StringSplit("key1=value1;key2=value2", ";");
        std::cout << "String Split: ";
        for (const auto& token : tokens) {
            std::cout << token << " | ";
        }
        std::cout << std::endl;

        // Example: Convert between Wide and Multibyte Strings
        std::wstring wideStr = CommonFunctions::ConvMBSToWCS("Hello");
        std::wcout << L"Wide String: " << wideStr << std::endl;
        std::string narrowStr = CommonFunctions::ConvWCSToMBS(wideStr);
        std::cout << "Wide to Narrow: " << narrowStr << std::endl;
        

        // Test strMatch
        bool match = CommonFunctions::strMatch("Hello", "H*o");
        std::cout << "strMatch: " << (match ? "true" : "false") << std::endl;
        match = CommonFunctions::strMatch("Hello", "Hello");
        std::cout << "strMatch: " << (match ? "true" : "false") << std::endl;

        // Test parseKeyValueString
        auto keyValueArray = CommonFunctions::parseKeyValueString("key1=value1;key2=value2");
        for (const auto& kv : keyValueArray) {
            std::cout << "Key: " << kv.first << " Value: " << kv.second << std::endl;
        }
        
        // Test tokenize
        auto fruitTokens = CommonFunctions::tokenize("apple,banana,grape", ",");
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


        // Example: EBCDIC to ASCII Conversion
        std::vector<unsigned char> ebcdicBuffer = {0xC1, 0xC2, 0xC3};  // Example EBCDIC data
        CommonFunctions::ebcdicToAscii(ebcdicBuffer);
        std::cout << "EBCDIC to ASCII: ";
        for (unsigned char byte : ebcdicBuffer) {
            std::cout << byte << " ";
        }
        std::cout << std::endl;

        // Example: Generate GUID
        std::string guid = CommonFunctions::GetGUID();
        std::cout << "Generated GUID: " << guid << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }


    return 0;
}

