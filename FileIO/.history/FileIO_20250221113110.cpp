#include "FileIO.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <cstdarg>
#include <system_error>
#include <cerrno>
#include <iostream>

const int CFileIO::FLAG_CREATE_NEW = std::ios::trunc;
const int CFileIO::FLAG_CREATE_ALWAYS = std::ios::trunc;
const int CFileIO::FLAG_OPEN_EXISTING = std::ios::in;
const int CFileIO::FLAG_OPEN_ALWAYS = std::ios::in | std::ios::out;
const int CFileIO::FLAG_TRUNCATE_EXISTING = std::ios::trunc;

CFileIO::CFileIO(std::string &fileName, int flags)
{
    FilePos = 0;
    this->cFileName = fileName;
    
    fileStream.open(fileName, flags | std::ios::out);
    if (!fileStream.is_open())
    {
        throw std::system_error(errno, std::generic_category(), "CFileIO: Unable to open file: " + fileName);
    }
    fileStream.seekg(0, std::ios::beg);
}

CFileIO::~CFileIO()
{
    if (fileStream.is_open())
    {
        fileStream.close();
    }
}

std::streampos CFileIO::FileSeek(std::streamoff offset, std::ios_base::seekdir direction)
{
    fileStream.clear();
    fileStream.seekg(offset, direction);
    return fileStream.tellg();
}

long CFileIO::WriteLine(const std::string &buffer)
{
    std::lock_guard<std::mutex> lock(fileMutex);
    if (!fileStream.is_open())
    {
        return -1;
    }

    fileStream << buffer << "\n"; // Use `\n` for cross-platform compatibility

    if (fileStream.fail())
    {
        return -1;
    }

    return buffer.length() + 1;
}

long CFileIO::ReadLine(std::string &buffer)
{
    std::lock_guard<std::mutex> lock(fileMutex);
    if (!fileStream.is_open())
    {
        return -1;
    }

    buffer.clear();

    if (!std::getline(fileStream, buffer))
    {
        return fileStream.eof() ? 0 : -1;
    }

    return buffer.length();
}

void CFileIO::flush()
{
    std::lock_guard<std::mutex> lock(fileMutex);
    if (fileStream.is_open())
    {
        fileStream.flush();
    }
}

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
    return fileStream.is_open() ? fileStream.tellg() : -1;
}
