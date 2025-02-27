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


// Uses std::vector<std::string> instead of char**
int CFileOps::ReadDelimitedLine(std::vector<std::string>& vecBuffer, std::string_view delimiter, unsigned int maxSize) {
    if (!fileStream.is_open() || fileStream.eof()) {
        return -1;
    }

    std::string line;
    if (!std::getline(fileStream, line)) {
        return fileStream.eof() ? 0 : -1;
    }

    line.erase(line.find_last_not_of("\n\r") + 1); // Trim newline characters

    size_t pos = 0;
    while ((pos = line.find_first_of(delimiter)) != std::string::npos) {
        vecBuffer.emplace_back(line.substr(0, pos));
        line.erase(0, pos + 1);
    }
    vecBuffer.emplace_back(line);

    return static_cast<int>(vecBuffer.size());
}

int CFileOps::ReadParamLine(std::vector<std::string>& vecBuffer, unsigned int maxSize) {
    return ReadDelimitedLine(vecBuffer, "\t ", maxSize);
}

std::vector<unsigned char> CFileOps::ReadBinary(const char* filename) {

    std::streamsize fileSize = tellg();
    seekg(0, std::ios::beg);

    std::vector<unsigned char> fileData(fileSize);
    
    if (!read(reinterpret_cast<char*>(fileData.data()), fileSize)) {
        throw std::runtime_error("Error: Failed to read file " + std::string(filename));
    }

    return fileData;
}

