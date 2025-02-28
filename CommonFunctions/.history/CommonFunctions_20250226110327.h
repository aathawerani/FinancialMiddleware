#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <span>
#include <windows.h>
#include <optional>
#include <atomic>

class CommonFunctions {
public:
    static constexpr size_t MAX_STRING_LENGTH = 256;

    struct StructKeyValue {
        char key[MAX_STRING_LENGTH]{};
        char value[MAX_STRING_LENGTH]{};
    };

    [[nodiscard]] static std::string BinToHex(std::span<const unsigned char> buffer);
    [[nodiscard]] static std::string HexToBin(std::string_view hex);

    [[nodiscard]] static std::vector<std::string_view> StringSplit(std::string_view source, 
                                                                   std::string_view delimiter, 
                                                                   bool keepEmpty = false);
    
    static bool ConvWCSToMBS(char* cStr, const wchar_t* wStr, unsigned int& uiBytes, size_t maxSize);
    static bool ConvMBSToWCS(wchar_t* wStr, const char* cStr, unsigned int& uiBytes);
    
    struct StructKeyValue {
        char key[256];
        char value[256];
    };
    static int parseKeyValueString(char* cString, StructKeyValue keyValueArray[]);
    static int hexToAscii(std::span<unsigned char> buffer);
    static int asciiToHex(std::span<unsigned char> buffer);
    static int ebcdicToAscii(std::span<unsigned char> buffer);
    static int asciiToEbcdic(std::span<unsigned char> buffer);
    std::string GetGUID();

    static bool strMatch(std::string_view src, std::string_view dest);

    static int parseKeyValueString(std::string_view str, std::vector<StructKeyValue>& keyValueArray);
    
    static std::vector<std::string> tokenize(std::string_view str, std::string_view delimiters);
    
    static void timeStamp(std::string& timeStr, SYSTEMTIME& systemTime);
};

