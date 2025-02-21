#ifndef FILEOPS_H
#define FILEOPS_H

#include <fstream>
#include <mutex>
#include <vector>
#include <string>
#include <string_view>
#include <span>
#include "../FileIO/FileIO.h"

class CFileOps : public CFileIO {
public:
    CFileOps(std::string_view cFileName, int iFlag);
    ~CFileOps() = default;

    int WriteParamLine(std::string_view format, ...);
    int ReadDelimitedLine(std::span<char> buffer, std::string_view delimiter, unsigned int maxSize);
    int ReadParamLine(std::span<char> buffer, unsigned int maxSize);
    int ReadDelimitedLine(std::vector<std::string>& vecBuffer, std::string_view delimiter, unsigned int maxSize);
    int ReadParamLine(std::vector<std::string>& vecBuffer, unsigned int maxSize);

};

#endif // FILEOPS_H
