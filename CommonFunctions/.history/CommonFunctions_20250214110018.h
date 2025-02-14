#include <string>
#include <vector>

class CommonFunctions
{
public:

	static std::string BinToHex(const unsigned char* buffer, unsigned int size);
	static std::vector<unsigned char> HexToBin(const unsigned char * buffer, unsigned int size);

	static std::vector<std::string> StringSplit(const std::string& source,
		const char *delimiter, bool keepEmpty = false);

};