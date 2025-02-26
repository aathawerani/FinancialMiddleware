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

