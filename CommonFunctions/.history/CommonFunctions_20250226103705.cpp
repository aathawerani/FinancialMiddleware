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

std::optional<std::pair<std::string, int>> CommonFunctions::getIPPort(std::string_view address) {
    size_t pos = address.rfind(':');
    if (pos == std::string::npos) return std::nullopt;
    
    std::string ip = std::string(address.substr(0, pos));
    int port = std::stoi(std::string(address.substr(pos + 1)));
    return std::make_pair(ip, port);
}

std::vector<uint8_t> CommonFunctions::ReadBinary(std::string_view filepath) {
    return {}; // Placeholder for file reading logic
}

void CommonFunctions::timeStamp(std::string& timeStr, SYSTEMTIME& systemTime) {
    char buffer[20];
    sprintf_s(buffer, "%04d-%02d-%02d %02d:%02d:%02d", 
              systemTime.wYear, systemTime.wMonth, systemTime.wDay, 
              systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
    timeStr = buffer;
}
