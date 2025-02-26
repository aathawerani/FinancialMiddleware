#include "CommonFunctions.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <charconv>
#include <span>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <cwchar>
#include <memory>


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





bool CommonFunctions::strMatch(const char* src, const char* dest) {
    if (*dest == '*') return true;
    
    while (*src && *dest) {
        if (*dest == '?') {
            ++dest;
            ++src;
            continue;
        }
        if (*dest == '*') return true;
        if (*dest != *src) return false;
        ++dest;
        ++src;
    }
    return (*dest == '\0' && *src == '\0');
}

bool CommonFunctions::ConvWCSToMBS(std::string& cStr, const std::wstring& wStr) {
    size_t len = wcstombs(nullptr, wStr.c_str(), 0);
    if (len == static_cast<size_t>(-1)) return false;

    cStr.resize(len);
    return wcstombs(cStr.data(), wStr.c_str(), len) != static_cast<size_t>(-1);
}

bool CommonFunctions::ConvMBSToWCS(std::wstring& wStr, const std::string& cStr) {
    size_t len = mbstowcs(nullptr, cStr.c_str(), 0);
    if (len == static_cast<size_t>(-1)) return false;

    wStr.resize(len);
    return mbstowcs(wStr.data(), cStr.c_str(), len) != static_cast<size_t>(-1);
}


std::vector<KeyValue> CommonFunctions::parseKeyValueString(const std::string& input) {
    std::vector<KeyValue> keyValuePairs;
    size_t start = 0, end = 0;

    while ((end = input.find(';', start)) != std::string::npos) {
        std::string pair = input.substr(start, end - start);
        size_t sep = pair.find('=');
        if (sep != std::string::npos) {
            keyValuePairs.push_back({pair.substr(0, sep), pair.substr(sep + 1)});
        }
        start = end + 1;
    }
    
    size_t sep = input.find('=', start);
    if (sep != std::string::npos) {
        keyValuePairs.push_back({input.substr(start, sep - start), input.substr(sep + 1)});
    }
    return keyValuePairs;
}

bool CommonFunctions::asciiToHex(std::vector<unsigned char>& buffer) {
    if (buffer.size() % 2 != 0) return false;

    std::vector<unsigned char> hexBuffer(buffer.size() / 2);
    for (size_t i = 0; i < hexBuffer.size(); ++i) {
    int high = buffer[i * 2] > '9' ? buffer[i * 2] - 'A' + 10 : buffer[i * 2] - '0';
    int low = buffer[i * 2 + 1] > '9' ? buffer[i * 2 + 1] - 'A' + 10 : buffer[i * 2 + 1] - '0';
    if (high < 0 || high > 15 || low < 0 || low > 15) return false;
    hexBuffer[i] = (high << 4) | low;
    }
    buffer = std::move(hexBuffer);
    return true;
}

void CommonFunctions::hexToAscii(std::vector<unsigned char>& buffer) {
    std::vector<unsigned char> asciiBuffer(buffer.size() * 2);
    for (size_t i = 0; i < buffer.size(); ++i) {
        asciiBuffer[i * 2] = "0123456789ABCDEF"[(buffer[i] >> 4) & 0xF];
        asciiBuffer[i * 2 + 1] = "0123456789ABCDEF"[buffer[i] & 0xF];
    }
    buffer = std::move(asciiBuffer);
}

