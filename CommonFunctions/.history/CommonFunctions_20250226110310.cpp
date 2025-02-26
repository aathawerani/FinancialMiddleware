#include "CommonFunctions.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <charconv>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <cstring>

std::string CommonFunctions::BinToHex(std::span<const unsigned char> buffer) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    
    for (unsigned char byte : buffer) {
        oss << std::setw(2) << static_cast<int>(byte);
    }
    return oss.str();
}

std::string CommonFunctions::HexToBin(std::string_view hex) {
    if (hex.length() % 2 != 0) {
        throw std::invalid_argument("Hex string must have an even length.");
    }

    std::string bin;
    bin.reserve(hex.length() / 2);

    for (size_t i = 0; i < hex.length(); i += 2) {
        int value;
        auto [ptr, ec] = std::from_chars(hex.data() + i, hex.data() + i + 2, value, 16);
        if (ec != std::errc()) {
            throw std::invalid_argument("Invalid hex character.");
        }
        bin.push_back(static_cast<char>(value));
    }

    return bin;
}

std::vector<std::string_view> CommonFunctions::StringSplit(std::string_view source, 
                                                           std::string_view delimiter, 
                                                           bool keepEmpty) {
    std::vector<std::string_view> results;
    size_t start = 0, end;

    while ((end = source.find(delimiter, start)) != std::string::npos) {
        if (keepEmpty || (end > start)) {
            results.emplace_back(source.substr(start, end - start));
        }
        start = end + delimiter.size();
    }

    if (keepEmpty || (start < source.size())) {
        results.emplace_back(source.substr(start));
    }

    return results;
}

bool CommonFunctions::strMatch(std::string_view src, std::string_view dest) {
    return src == dest;
}

void CommonFunctions::timeStamp(std::string& timeStr, SYSTEMTIME& systemTime) {
    char buffer[20];
    sprintf_s(buffer, "%04d-%02d-%02d %02d:%02d:%02d", 
              systemTime.wYear, systemTime.wMonth, systemTime.wDay, 
              systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
    timeStr = buffer;
}

bool CommonFunctions::ConvWCSToMBS(char* cStr, const wchar_t* wStr, unsigned int& uiBytes, size_t maxSize) {
    int rc = wcstombs(cStr, wStr, uiBytes);
    if (rc < 0) return false;
    cStr[rc] = '\0';
    return true;
}

bool CommonFunctions::ConvMBSToWCS(wchar_t* wStr, const char* cStr, unsigned int& uiBytes) {
    int rc = mbstowcs(wStr, cStr, uiBytes);
    if (rc < 0) return false;
    wStr[rc] = L'\0';
    return true;
}

int CommonFunctions::parseKeyValueString(char* cString, StructKeyValue keyValueArray[]) {
    int parseCount = 0;
    char* pair = strtok(cString, ";");
    while (pair) {
        char* separator = strchr(pair, '=');
        if (separator) {
            *separator = '\0';
            strncpy(keyValueArray[parseCount].key, pair, sizeof(keyValueArray[parseCount].key));
            strncpy(keyValueArray[parseCount].value, separator + 1, sizeof(keyValueArray[parseCount].value));
            ++parseCount;
        }
        pair = strtok(nullptr, ";");
    }
    return parseCount;
}

int CommonFunctions::ebcdicToAscii(const std::string& ebcdicStr) {
    static const unsigned char EbcdicToAsciiTable[256] = {
        0x00, 0x01, 0x02, 0x03, 0x9C, 0x09, 0x86, 0x7F, 0x97, 0x8D, 0x8E, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x9D, 0x85, 0x08, 0x87, 0x18, 0x19, 0x92, 0x8F, 0x1C, 0x1D, 0x1E, 0x1F,
        0x80, 0x81, 0x82, 0x83, 0x84, 0x0A, 0x17, 0x1B, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x05, 0x06, 0x07,
        0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04, 0x98, 0x99, 0x9A, 0x9B, 0x14, 0x15, 0x9E, 0x1A,
        0x20, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xD5, 0x2E, 0x3C, 0x28, 0x2B, 0x7C,
    };

    std::string asciiStr;
    asciiStr.reserve(ebcdicStr.size());

    for (unsigned char ch : ebcdicStr) {
        asciiStr.push_back(EbcdicToAsciiTable[ch]);
    }

    return asciiStr;
}

int CommonFunctions::asciiToEbcdic(const std::string& ebcdicStr) {
    static const unsigned char AsciiToEbcdicTable[256] = {
        0x00, 0x01, 0x02, 0x03, 0x37, 0x2D, 0x2E, 0x2F, 0x16, 0x05, 0x25, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x3C, 0x3D, 0x32, 0x26, 0x18, 0x19, 0x3F, 0x27, 0x1C, 0x1D, 0x1E, 0x1F,
        0x40, 0x5A, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D, 0x4D, 0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61,
        0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0x7A, 0x5E, 0x4C, 0x7E, 0x6E, 0x6F,
    };

    std::string ebcdicStr;
    ebcdicStr.reserve(asciiStr.size());

    for (unsigned char ch : asciiStr) {
        ebcdicStr.push_back(AsciiToEbcdicTable[ch]);
    }

    return ebcdicStr;
}

std::string CommonFunctions::GetGUID() {
    UUID uuid;
    UuidCreate(&uuid);
    RPC_CSTR uuidString;
    UuidToStringA(&uuid, &uuidString);
    std::string guid(reinterpret_cast<char*>(uuidString));
    RpcStringFreeA(&uuidString);
    
    for (char& ch : guid) {
        ch = static_cast<char>(std::toupper(ch));
    }
    return guid;
}
