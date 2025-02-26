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

#pragma comment(lib, "Crypt32.lib")

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

