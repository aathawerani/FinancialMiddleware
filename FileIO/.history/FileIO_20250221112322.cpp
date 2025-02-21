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
    fileStream.clear();
    fileStream.seekg(distance, direction);
    return fileStream.tellg();
}

long CFileIO::WriteLine(const std::string &buffer)
{
    std::lock_guard<std::mutex> lock(fileMutex);
    if (!fileStream.is_open())
    {
        return -1;
    }

    fileStream << buffer << "\r\n"; // Write the line with newline

    if (fileStream.fail())
    {
        return -1;
    }

    return buffer.length() + 2; // Return length including `\r\n`
}

long CFileIO::ReadLine(std::string &buffer)
{
    std::lock_guard<std::mutex> lock(fileMutex);
    if (!fileStream.is_open())
    {
        return -1;
    }

    buffer.clear(); // Clear previous content

    if (!std::getline(fileStream, buffer))
    {
        if (fileStream.eof())
            return 0; // End of file reached
        return -1; // Some other failure
    }

    return buffer.length(); // Return the length of the line read
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

void CFileIO::clear()
{
    std::lock_guard<std::mutex> lock(fileMutex);
    if (fileStream.is_open())
    {
        fileStream.clear();
    }
}

std::streampos CFileIO::getFpos()
{
    if (fileStream.is_open())
    {
        return fileStream.tellg();
    }
    return -1; // Return an invalid position if the file is not open
}
