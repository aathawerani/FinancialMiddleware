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
    std::unique_ptr<char[]> acBase64 = std::make_unique<char[]>(size * 2);
    DWORD len = size * 2;
    if (CryptBinaryToStringA((const BYTE*)buffer, size, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, acBase64.get(), &len) == FALSE)
        return "";
    return std::string(acBase64.get(), len);
}

std::string CommonFunctions::HexToBin(const char* hex, unsigned int length) {
    std::string bin;
    bin.reserve(length / 2);
    for (unsigned int i = 0; i < length; i += 2) {
        std::string byte = std::string(hex, i, 2);
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
