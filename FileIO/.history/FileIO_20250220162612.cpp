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

long CFileIO::WriteLine(const char *cBuffer, int iBufferLen)
{
    std::lock_guard<std::mutex> lock(fileMutex);
    if (!fileStream.is_open())
    {
        return -1;
    }
    fileStream.write(cBuffer, iBufferLen);
    fileStream.write("\n", 1); // Add newline character
    if (fileStream.fail())
    {
        return -1;
    }
    return iBufferLen + 1; // Include the newline character in the length
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

    // Get the length of the line read
    long length = strlen(cBuffer);

    // Check if the last character read was a newline character
    if (fileStream.peek() == '\n' || fileStream.peek() == '\r')
    {
        // Read the newline character(s) and append to the buffer
        while (fileStream.peek() == '\n' || fileStream.peek() == '\r')
        {
            cBuffer[length++] = fileStream.get();
        }
        cBuffer[length] = '\0'; // Null-terminate the buffer
    }

    return length; // Return the length of the string read into the buffer
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
