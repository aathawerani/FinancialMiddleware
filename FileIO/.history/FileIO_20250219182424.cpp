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

const int CFileIO::FLAG_CREATE_NEW = std::ios::trunc;
const int CFileIO::FLAG_CREATE_ALWAYS = std::ios::trunc;
const int CFileIO::FLAG_OPEN_EXISTING = std::ios::in;
const int CFileIO::FLAG_OPEN_ALWAYS = std::ios::in | std::ios::out;
const int CFileIO::FLAG_TRUNCATE_EXISTING = std::ios::trunc;

CFileIO::CFileIO(const char *cFileName, int iFlag)
{
	std::cout << "got here 1" << std::endl;
    FilePos = 0;
    this->cFileName = cFileName;
	std::cout << "got here 2" << std::endl;
    fileStream.open(cFileName, iFlag);
	std::cout << "got here 3" << std::endl;
    if (!fileStream.is_open())
    {
		std::cout << "got here 4" << std::endl;
        std::error_code ec(errno, std::generic_category());
		std::cout << "error " << errno << std::endl;
        throw std::runtime_error(std::string("CFileIO :: Unable to open file: ") + cFileName + " - " + ec.message());
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
    fileStream.getline(cBuffer, 1000);
    if (fileStream.fail() && !fileStream.eof())
    {
        return -1;
    }
    return fileStream.gcount();
}

int CFileIO::WriteParamLine(const char* cFormat, ...)
{
    static char tempStr[MAX_FILE_LENGTH];
    static char logCharString[MAX_FILE_LENGTH];

    va_list arglist;
    va_start(arglist, cFormat);
    vsnprintf(logCharString, MAX_FILE_LENGTH, cFormat, arglist);
    va_end(arglist);

    snprintf(tempStr, MAX_FILE_LENGTH, "%s\r\n", logCharString);
    return WriteLine(tempStr, strlen(tempStr));
}

int CFileIO::ReadDelimitedLine(char **cBuffer, const char *cDelimitedStr, unsigned int maxSize)
{
    std::unique_ptr<char[]> atempStr(new char[maxSize]);
    long iBytesRead;

    if (FilePos == FileSeek(0, std::ios::end)) // File Processed
        return 0;

    FileSeek(FilePos, std::ios::beg);
    iBytesRead = ReadLine(atempStr.get());
    if (iBytesRead <= 0)
        return 0;

    atempStr[iBytesRead] = 0;
    char *cParamString = atempStr.get();
    char *cPos = nullptr;

    while ((cPos = strpbrk(cParamString, "\n\r")))
    {
        if (cPos - cParamString == 0)
        {
            cParamString++;
            FilePos += 1;
            continue;
        }
        else
            break;
    }
    if (cPos == nullptr)
    {
        cPos = cParamString + strlen(cParamString);
    }

    FilePos += (cPos - cParamString);
    cParamString[cPos - cParamString] = 0;

    if (cParamString[0] == '#')
        return -1;

    for (int j = strlen(cParamString) - 1; j > 0 && cParamString[j] == ' '; j--)
        cParamString[j] = 0;

    cPos = nullptr;
    int iParamsRead = 0;

    while ((cPos = strpbrk(cParamString, cDelimitedStr)))
    {
        if (cPos - cParamString == 0)
        {
            cParamString++;
            continue;
        }

        strncpy(cBuffer[iParamsRead], cParamString, cPos - cParamString);
        cBuffer[iParamsRead++][cPos - cParamString] = 0;
        cParamString = cPos + 1;
    }
    strcpy(cBuffer[iParamsRead], cParamString);

    return iParamsRead + 1;
}

int CFileIO::ReadParamLine(char **cBuffer, unsigned int maxSize)
{
    return ReadDelimitedLine(cBuffer, "\t ", maxSize);
}

int CFileIO::ReadDelimitedLine(std::vector<std::string> &vecBuffer, const char *cDelimitedStr, unsigned int maxSize)
{
    std::unique_ptr<char[]> aTempStr(new char[maxSize]);
    long iBytesRead;

    if (FilePos == FileSeek(0, std::ios::end)) // File Processed
        return 0;

    FileSeek(FilePos, std::ios::beg);
    iBytesRead = ReadLine(aTempStr.get());
    if (iBytesRead <= 0)
        return 0;

    aTempStr[iBytesRead] = 0;
    char *cParamString = aTempStr.get();
    char *cPos = nullptr;

    while ((cPos = strpbrk(cParamString, "\n\r")))
    {
        if (cPos - cParamString == 0)
        {
            cParamString++;
            FilePos += 1;
            continue;
        }
        else
            break;
    }
    if (cPos == nullptr)
    {
        cPos = cParamString + strlen(cParamString);
    }

    FilePos += (cPos - cParamString);
    cParamString[cPos - cParamString] = 0;

    if (cParamString[0] == '#')
        return -1;

    for (int j = strlen(cParamString) - 1; j > 0 && cParamString[j] == ' '; j--)
        cParamString[j] = 0;

    cPos = nullptr;

    while ((cPos = strpbrk(cParamString, cDelimitedStr)))
    {
        if (cPos - cParamString == 0)
        {
            cParamString++;
            continue;
        }

        strncpy(aTempStr.get(), cParamString, cPos - cParamString);
        aTempStr[cPos - cParamString] = 0;
        vecBuffer.push_back(aTempStr.get());
        cParamString = cPos + 1;
    }

    strcpy(aTempStr.get(), cParamString);
    vecBuffer.push_back(aTempStr.get());

    return vecBuffer.size();
}

int CFileIO::ReadParamLine(std::vector<std::string> &vecBuffer, unsigned int maxSize)
{
    return ReadDelimitedLine(vecBuffer, "\t ", maxSize);
}

int CFileIO::ReadFullLine(std::string &sOut)
{
    std::vector<std::string> vecBuffer;
    int rc = ReadDelimitedLine(vecBuffer, "\r\n", MAX_STRING_LENGTH * 4);

    if (!vecBuffer.empty())
        sOut = vecBuffer[0];

    return rc;
}
