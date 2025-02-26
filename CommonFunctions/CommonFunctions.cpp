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

const std::array<unsigned char, 256> EBCDIC_TO_ASCII = {
    /* 0x00 - 0x0F */ 0x00, 0x01, 0x02, 0x03, 0x9C, 0x09, 0x86, 0x7F, 0x97, 0x8D, 0x8E, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    /* 0x10 - 0x1F */ 0x10, 0x11, 0x12, 0x13, 0x9D, 0x85, 0x08, 0x87, 0x18, 0x19, 0x92, 0x8F, 0x1C, 0x1D, 0x1E, 0x1F,
    /* 0x20 - 0x2F */ 0x80, 0x81, 0x82, 0x83, 0x84, 0x0A, 0x17, 0x1B, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x05, 0x06, 0x07,
    /* 0x30 - 0x3F */ 0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04, 0x98, 0x99, 0x9A, 0x9B, 0x14, 0x15, 0x9E, 0x1A,
    /* 0x40 - 0x4F */ 0x20, 0xA0, 0xE2, 0xE4, 0xE0, 0xE1, 0xE3, 0xE5, 0xE7, 0xF1, 0xA2, 0x2E, 0x3C, 0x28, 0x2B, 0x7C,
    /* 0x50 - 0x5F */ 0x26, 0xE9, 0xEA, 0xEB, 0xE8, 0xED, 0xEE, 0xEF, 0xEC, 0xDF, 0x21, 0x24, 0x2A, 0x29, 0x3B, 0xAC,
    /* 0x60 - 0x6F */ 0x2D, 0x2F, 0xC2, 0xC4, 0xC0, 0xC1, 0xC3, 0xC5, 0xC7, 0xD1, 0xA6, 0x2C, 0x25, 0x5F, 0x3E, 0x3F,
    /* 0x70 - 0x7F */ 0xF8, 0xC9, 0xCA, 0xCB, 0xC8, 0xCD, 0xCE, 0xCF, 0xCC, 0x60, 0x3A, 0x23, 0x40, 0x27, 0x3D, 0x22,
    /* 0x80 - 0x8F */ 0xD8, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0xAB, 0xBB, 0xF0, 0xFD, 0xFE, 0xB1,
    /* 0x90 - 0x9F */ 0xB0, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0xAA, 0xBA, 0xE6, 0xB8, 0xC6, 0xA4,
    /* 0xA0 - 0xAF */ 0xB5, 0x7E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0xA1, 0xBF, 0xD0, 0xDD, 0xDE, 0xAE,
    /* 0xB0 - 0xBF */ 0x5E, 0xA3, 0xA5, 0xB7, 0xA9, 0xA7, 0xB6, 0xBC, 0xBD, 0xBE, 0x5B, 0x5D, 0xAF, 0xA8, 0xB4, 0xD7,
    /* 0xC0 - 0xCF */ 0x7B, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0xAD, 0xF4, 0xF6, 0xF2, 0xF3, 0xF5,
    /* 0xD0 - 0xDF */ 0x7D, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0xB9, 0xFB, 0xFC, 0xF9, 0xFA, 0xFF,
    /* 0xE0 - 0xEF */ 0x5C, 0xF7, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0xB2, 0xD4, 0xD6, 0xD2, 0xD3, 0xD5,
    /* 0xF0 - 0xFF */ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xB3, 0xDB, 0xDC, 0xD9, 0xDA, 0x9F
};


const std::array<unsigned char, 256> ASCII_TO_EBCDIC = {
    /* 0x00 - 0x0F */ 0x00, 0x01, 0x02, 0x03, 0x37, 0x2D, 0x2E, 0x2F, 0x16, 0x05, 0x25, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    /* 0x10 - 0x1F */ 0x10, 0x11, 0x12, 0x13, 0x3C, 0x32, 0x26, 0x18, 0x19, 0x3F, 0x27, 0x1C, 0x1D, 0x1E, 0x1F, 0x40,
    /* 0x20 - 0x2F */ 0x4F, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D, 0x4D, 0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61, 0xF0,
    /* 0x30 - 0x3F */ 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0x7A, 0x5E, 0x4C, 0x7E, 0x6E, 0x6F, 0x7C,
    /* 0x40 - 0x4F */ 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    /* 0x50 - 0x5F */ 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xBA, 0xE0, 0xBB, 0xB0, 0x6D, 0x79,
    /* 0x60 - 0x6F */ 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    /* 0x70 - 0x7F */ 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xC0, 0x4A, 0xE1, 0xBE, 0xE0, 0x6A,
    /* 0x80 - 0x8F */ 0xC0, 0x6A, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5,
    /* 0x90 - 0x9F */ 0xD6, 0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xBA, 0xE0, 0xBB, 0xB0,
    /* 0xA0 - 0xAF */ 0x6D, 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95,
    /* 0xB0 - 0xBF */ 0x96, 0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xC0, 0x4A, 0xE1, 0xBE,
    /* 0xC0 - 0xCF */ 0xE0, 0x6A, 0xC0, 0x6A, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xD1, 0xD2, 0xD3,
    /* 0xD0 - 0xDF */ 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xBA, 0xE0,
    /* 0xE0 - 0xEF */ 0xBB, 0xB0, 0x6D, 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x91, 0x92, 0x93,
    /* 0xF0 - 0xFF */ 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xC0, 0x4A
};

int CommonFunctions::ebcdicToAscii(std::span<unsigned char> buffer) {
    for (auto& byte : buffer) {
        unsigned char original = byte;
        byte = EBCDIC_TO_ASCII[byte];
        std::cout << "EBCDIC: " << static_cast<int>(original)
                  << " -> ASCII: " << static_cast<int>(byte) 
                  << " (" << static_cast<char>(byte) << ")\n";
    }
    return 1;
}

int CommonFunctions::asciiToEbcdic(std::span<unsigned char> buffer) {
    for (auto& byte : buffer) {
        unsigned char original = byte;
        byte = ASCII_TO_EBCDIC[byte];
        std::cout << "ASCII: " << static_cast<int>(original)
                  << " -> EBCDIC: " << static_cast<int>(byte) 
                  << " (" << static_cast<char>(byte) << ")\n";

        if (ASCII_TO_EBCDIC[0x41] != 0xC1 || ASCII_TO_EBCDIC[0x42] != 0xC2 || ASCII_TO_EBCDIC[0x43] != 0xC3) {
            std::cerr << "Error: ASCII_TO_EBCDIC table is incorrect!\n";
            std::cerr << "ASCII_TO_EBCDIC[0x41] = " << std::hex << static_cast<int>(ASCII_TO_EBCDIC[0x41]) << "\n";
            std::cerr << "ASCII_TO_EBCDIC[0x42] = " << std::hex << static_cast<int>(ASCII_TO_EBCDIC[0x42]) << "\n";
            std::cerr << "ASCII_TO_EBCDIC[0x43] = " << std::hex << static_cast<int>(ASCII_TO_EBCDIC[0x43]) << "\n";
        }
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
