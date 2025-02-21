#include <string>
#include <vector>
#include <string_view>
#include <span>

class CommonFunctions
{
public:
    [[nodiscard]] static std::string BinToHex(std::span<const unsigned char> buffer);
    [[nodiscard]] static std::string HexToBin(std::string_view hex);

    [[nodiscard]] static std::vector<std::string_view> StringSplit(std::string_view source,
                                                                   std::string_view delimiter,
                                                                   bool keepEmpty = false);
};
