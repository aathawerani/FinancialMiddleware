#include "FileIO.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <cstdarg> // Include for va_start and va_end
#include <system_error> // Include for system error codes
#include <cerrno> // Include for errno
#include <iostream>
#include <cstring>

const int CFileIO::FLAG_CREATE_NEW = std::ios::trunc;
const int CFileIO::FLAG_CREATE_ALWAYS = std::ios::trunc;
const int CFileIO::FLAG_OPEN_EXISTING = std::ios::in;
const int CFileIO::FLAG_OPEN_ALWAYS = std::ios::in | std::ios::out;
const int CFileIO::FLAG_TRUNCATE_EXISTING = std::ios::trunc;

CFileIO::CFileIO(const char *cFileName, int iFlag)
{
    FilePos = 0;
    this->cFileName = cFileName;
    fileStream.open(cFileName, iFlag | std::ios::out);
    if (!fileStream.is_open())
    {
        throw std::system_error(errno, std::generic_category(), "CFileIO :: Unable to open file: " + std::string(cFileName));
    }
    fileStream.seekg(0, std::ios::beg);
}

CFileIO::~CFileIO(void)
{
    if (fileStream.is_open())
    {
        fileStream.close();
    }
}

std::streampos CFileIO::FileSeek(std::streamoff distance, std::ios_base::seekdir direction)
{
    fileStream.seekg(distance, direction);
    return fileStream.tellg();
}

long CFileIO::WriteLine(const char *cBuffer, int iBufferLen)
{
    std::lock_guard<std::mutex> lock(fileMutex);
    if (!fileStream.is_open())
    {
        return -1;
    }
    fileStream.write(cBuffer, iBufferLen);
    if (fileStream.fail())
    {
        return -1;
    }
    return iBufferLen;
}

long CFileIO::ReadLine(char *cBuffer)
{
    std::lock_guard<std::mutex> lock(fileMutex);
    if (!fileStream.is_open())
    {
        return -1;
    }

    // Read a line from the file
    fileStream.getline(cBuffer, 1000);

    // Check for failure and handle it
    if (fileStream.fail() && !fileStream.eof())
    {
        return -1;
    }

    // Skip any remaining newline characters
    while (fileStream.peek() == '\n' || fileStream.peek() == '\r')
    {
        fileStream.get();
    }

    return fileStream.gcount();
}

// Implement the flush function
void CFileIO::flush()
{
    std::lock_guard<std::mutex> lock(fileMutex);
    if (fileStream.is_open())
    {
        fileStream.flush();
    }
}

// Implement the close function
void CFileIO::close()
{
    std::lock_guard<std::mutex> lock(fileMutex);
    if (fileStream.is_open())
    {
        fileStream.close();
    }
}
