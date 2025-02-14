#include "CommonFunctions.h"
#include <iostream>
#include <vector>

int main() {
    // Test BinToHex
    const unsigned char binData[] = { 0x48, 0x65, 0x6C, 0x6C, 0x6F };
    std::string hexStr = CommonFunctions::BinToHex(binData, sizeof(binData));
    std::cout << "BinToHex: " << hexStr << std::endl;

    // Test HexToBin
    std::string binStr = CommonFunctions::HexToBin(hexStr.c_str(), hexStr.size());
    std::cout << "HexToBin: " << binStr << std::endl;

    // Test StringSplit
    std::string str = "Hello,World,This,Is,A,Test";
    std::vector<std::string> splitStr = CommonFunctions::StringSplit(str, ",");
    std::cout << "StringSplit: ";
    for (const auto& s : splitStr) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    return 0;
}
