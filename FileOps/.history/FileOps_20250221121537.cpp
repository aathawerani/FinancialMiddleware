#include "FileOps.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <mutex>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <stdexcept>
#include <cstdarg>    // va_list, va_start, va_end
#include <system_error>
#include <cerrno>
#include <iostream>
#include <cstring>

CFileOps::CFileOps(std::string_view cFileName, int iFlag) 
    : CFileIO(std::string(cFileName), iFlag) {}

// Uses std::string instead of char buffers
int CFileOps::WriteParamLine(std::string_view format, ...) {
    std::string tempStr;
    tempStr.resize(1024); // Adjust size as needed

    va_list args;
    va_start(args, format);
    vsnprintf(tempStr.data(), tempStr.size(), format.data(), args);
    va_end(args);

    return WriteLine(tempStr);
}

// Uses std::span<char> for safe buffer handling
int CFileOps::ReadDelimitedLine(std::span<char> buffer, std::string_view delimiter, unsigned int maxSize) {
    if (!fileStream.is_open() || fileStream.eof()) {
        return -1;
    }

    std::string tempStr;
    if (!std::getline(fileStream, tempStr)) {
        return fileStream.eof() ? 0 : -1;
    }

    tempStr.erase(tempStr.find_last_not_of("\n\r") + 1); // Trim newline characters

    size_t pos = 0;
    size_t count = 0;
    while ((pos = tempStr.find_first_of(delimiter)) != std::string::npos) {
        std::string token = tempStr.substr(0, pos);
        tempStr.erase(0, pos + 1);
        if (count < buffer.size()) {
            std::copy_n(token.begin(), std::min(token.size(), buffer.size()), buffer.begin() + count);
        }
        count++;
    }
    return count + 1;
}


int CFileOps::ReadParamLine(std::vector<std::string>& vecBuffer, unsigned int maxSize) {
    return ReadDelimitedLine(vecBuffer, "\t ", maxSize);
}

