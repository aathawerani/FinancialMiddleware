#include <string>
#include <vector>
#include <string_view>
#include <span>

class CommonFunctions
{
public:
    [[nodiscard]] static std::string BinToHex(std::span<const unsigned char> buffer);
    [[nodiscard]] static std::string HexToBin(std::string_view hex);

    static std::string GetMK(std::span<unsigned char> buffer, std::string_view key);
    static std::string base64(std::span<const unsigned char> input);

    [[nodiscard]] static std::vector<std::string_view> StringSplit(std::string_view source,
                                                                   std::string_view delimiter,
                                                                   bool keepEmpty = false);
};


class CommonFunctions {
public:
    static std::vector<std::string_view> StringSplit(std::string_view source, std::string_view delimiter, bool keepEmpty = false);
};
