#include <string>
#include <vector>
#include <string_view>
#include <span>
#include <windows.h>
#include <optional>
#include <atomic>

namespace {
    const std::array<unsigned char, 256> EBCDIC_TO_ASCII = {
        /* 0x00 - 0x0F */ 0x00, 0x01, 0x02, 0x03, 0x9C, 0x09, 0x86, 0x7F, 0x97, 0x8D, 0x8E, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        /* 0x10 - 0x1F */ 0x10, 0x11, 0x12, 0x13, 0x9D, 0x85, 0x08, 0x87, 0x18, 0x19, 0x92, 0x8F, 0x1C, 0x1D, 0x1E, 0x1F,
        /* 0x20 - 0x2F */ 0x80, 0x81, 0x82, 0x83, 0x84, 0x0A, 0x17, 0x1B, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x05, 0x06, 0x07,
        /* (other mappings omitted for brevity) */
    };

    const std::array<unsigned char, 256> ASCII_TO_EBCDIC = {
        /* (Mapping omitted, but follows a similar structure) */
    };
}

class CommonFunctions {
public:

    static constexpr size_t MAX_STRING_LENGTH = 256;

    static std::string BinToHex(std::span<const unsigned char> bin);
    static std::optional<std::string> HexToBin(const std::string& hex);
    static std::vector<std::string_view> StringSplit(std::string_view str, std::string_view delimiter);
    static std::wstring ConvMBSToWCS(std::string_view str);
    static std::string ConvWCSToMBS(std::wstring_view wstr);
    static std::vector<std::pair<std::string, std::string>> parseKeyValueString(std::string_view input);
    
    static int ebcdicToAscii(std::span<unsigned char> buffer);
    static int asciiToEbcdic(std::span<unsigned char> buffer);

    static std::string GetGUID();

    static int hexToAscii(std::span<unsigned char> buffer);
    static int asciiToHex(std::span<unsigned char> buffer);
    
    static std::vector<std::string> tokenize(std::string_view str, std::string_view delimiters);
    
    static void timeStamp(std::string& timeStr, SYSTEMTIME& systemTime);
};

