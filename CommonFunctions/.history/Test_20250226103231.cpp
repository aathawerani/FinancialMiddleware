#include "CommonFunctions.h"
#include <iostream>
#include <vector>

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

        std::string convertedStr;
        CUtilityFunctions::ConvWCSToMBS(convertedStr, L"Hello World");
        std::cout << "Converted WCS to MBS: " << convertedStr << std::endl;
    
        std::wstring convertedWStr;
        CUtilityFunctions::ConvMBSToWCS(convertedWStr, "Hello World");
        std::wcout << L"Converted MBS to WCS: " << convertedWStr << std::endl;
    
        auto keyValuePairs = CUtilityFunctions::parseKeyValueString("key1=value1;key2=value2;key3=value3");
        for (const auto& kv : keyValuePairs) {
            std::cout << "Key: " << kv.key << " Value: " << kv.value << std::endl;
        }
    
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
