#include "CommonFunctions.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <charconv>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <vector>
#include <optional>
#include <array>
#include <span>
#include <ranges>

#pragma comment(lib, "Rpcrt4.lib")

std::string CommonFunctions::BinToHex(std::span<const unsigned char> bin) {
    std::ostringstream oss;
    for (unsigned char byte : bin) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

std::optional<std::string> CommonFunctions::HexToBin(const std::string& hex) {
    if (hex.length() % 2 != 0) return std::nullopt; // Ensure valid hex string
    
    std::string bin;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteStr = hex.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byteStr, nullptr, 16));
        bin.push_back(byte);
    }
    return bin;
}

std::vector<std::string_view> CommonFunctions::StringSplit(std::string_view str, std::string_view delimiter) {
    std::vector<std::string_view> result;
    size_t pos = 0, nextPos;
    while ((nextPos = str.find(delimiter, pos)) != std::string_view::npos) {
        result.emplace_back(str.substr(pos, nextPos - pos));
        pos = nextPos + delimiter.size();
    }
    result.emplace_back(str.substr(pos));
    return result;
}

void CommonFunctions::timeStamp(std::string& timeStr, SYSTEMTIME& systemTime) {
    char buffer[20];
    sprintf_s(buffer, "%04d-%02d-%02d %02d:%02d:%02d", 
              systemTime.wYear, systemTime.wMonth, systemTime.wDay, 
              systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
    timeStr = buffer;
}

std::wstring CommonFunctions::ConvMBSToWCS(std::string_view str) {
    if (str.empty()) return {};

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
    if (size_needed <= 0) return {};

    std::wstring result(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), size_needed);
    
    return result;
}

std::string CommonFunctions::ConvWCSToMBS(std::wstring_view wstr) {
    if (wstr.empty()) return {};

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0) return {};

    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), result.data(), size_needed, nullptr, nullptr);
    
    return result;
}


std::vector<std::pair<std::string, std::string>> CommonFunctions::parseKeyValueString(std::string_view input) {
    std::vector<std::pair<std::string, std::string>> keyValueArray;
    size_t pos = 0, nextPos;

    while ((nextPos = input.find(';', pos)) != std::string_view::npos) {
        auto pair = input.substr(pos, nextPos - pos);
        size_t eqPos = pair.find('=');
        if (eqPos != std::string_view::npos) {
            keyValueArray.emplace_back(std::string(pair.substr(0, eqPos)), std::string(pair.substr(eqPos + 1)));
        }
        pos = nextPos + 1;
    }

    // Process the last key-value pair if there's no trailing semicolon
    if (pos < input.size()) {
        auto pair = input.substr(pos);
        size_t eqPos = pair.find('=');
        if (eqPos != std::string_view::npos) {
            keyValueArray.emplace_back(std::string(pair.substr(0, eqPos)), std::string(pair.substr(eqPos + 1)));
        }
    }

    return keyValueArray;
}

int CommonFunctions::ebcdicToAscii(std::span<unsigned char> buffer) {
    for (auto& byte : buffer) {
        byte = EBCDIC_TO_ASCII[byte];
    }
    return 1;
}

int CommonFunctions::asciiToEbcdic(std::span<unsigned char> buffer) {
    for (auto& byte : buffer) {
        byte = ASCII_TO_EBCDIC[byte];
    }
    return 1;
}

std::string CommonFunctions::GetGUID() {
    UUID uuid;
    UuidCreate(&uuid);
    RPC_CSTR uuidString;
    UuidToStringA(&uuid, &uuidString);
    std::string guid(reinterpret_cast<char*>(uuidString));
    RpcStringFreeA(&uuidString);
    
    std::ranges::transform(guid, guid.begin(), ::toupper);
    return guid;
}

std::vector<std::string> CommonFunctions::tokenize(std::string_view input, std::string_view delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0, nextPos;
    while ((nextPos = input.find(delimiter, pos)) != std::string_view::npos) {
        tokens.emplace_back(input.substr(pos, nextPos - pos));
        pos = nextPos + delimiter.size();
    }
    tokens.emplace_back(input.substr(pos));
    return tokens;
}
