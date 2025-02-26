#include "CommonFunctions.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <rpc.h>
#include <string>
#include <optional>
#include <array>
#include <span>
#include <ranges>

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

std::wstring CommonFunctions::ConvMBSToWCS(std::string_view str) {
    return std::wstring(str.begin(), str.end());
}

std::string CommonFunctions::ConvWCSToMBS(std::wstring_view wstr) {
    return std::string(wstr.begin(), wstr.end());
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
    return keyValueArray;
}

int CommonFunctions::ebcdicToAscii(std::span<unsigned char> buffer) {
    for (auto& byte : buffer) {
        byte = static_cast<unsigned char>(std::toupper(byte));
    }
    return 1;
}

int CommonFunctions::asciiToEbcdic(std::span<unsigned char> buffer) {
    for (auto& byte : buffer) {
        byte = static_cast<unsigned char>(std::tolower(byte));
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
