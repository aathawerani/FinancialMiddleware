#include "CommonFunctions.h"
#include <memory>
#include <vector>
#include <string>
#include <Windows.h>
#include <sstream>
#include <iomanip>
#pragma comment(lib, "Crypt32.lib")

std::string CommonFunctions::BinToHex(const unsigned char* buffer, unsigned int size)
{
    std::ostringstream oss;
    for (unsigned int i = 0; i < size; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)buffer[i];
    }
    return oss.str();
}

std::string CommonFunctions::HexToBin(const char* hex, unsigned int length) {
    std::string bin;
    bin.reserve(length / 2);
    for (unsigned int i = 0; i < length; i += 2) {
        std::string byte = std::string(hex + i, 2);
        char chr = (char)(int)strtol(byte.c_str(), nullptr, 16);
        bin.push_back(chr);
    }
    return bin;
}

std::vector<std::string> CommonFunctions::StringSplit(
    const std::string &source, const char *delimiter, bool keepEmpty /*= false*/)
{
    std::vector<std::string> results;

    size_t prev = 0;
    size_t next = 0;

    while ((next = source.find_first_of(delimiter, prev)) != std::string::npos)
    {
        if (keepEmpty || (next - prev != 0))
        {
            results.push_back(source.substr(prev, next - prev));
        }
        prev = next + 1;
    }

    if (prev < source.size())
    {
        results.push_back(source.substr(prev));
    }

    return results;
}
