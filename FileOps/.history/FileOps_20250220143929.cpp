#include "FileOps.h"
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

#include "../FileIO/FileIO.h"

CFileOps::CFileOps(const char *cFileName, int iFlag) : CFileIO(cFileName, iFlag) {
    // Constructor implementation
}

CFileOps::~CFileOps(void) {
    // Destructor implementation
}

int CFileOps::WriteParamLine(const char* cFormat, ...)
{
    static char tempStr[MAX_FILE_LENGTH];
    static char logCharString[MAX_FILE_LENGTH];

    va_list arglist;
    va_start(arglist, cFormat);
    vsnprintf(logCharString, MAX_FILE_LENGTH, cFormat, arglist);
    va_end(arglist);

    snprintf(tempStr, MAX_FILE_LENGTH, "%s\r\n", logCharString);
    std::cout << "tempStr: " << tempStr;
    return WriteLine(tempStr, strlen(tempStr));
}

int CFileOps::ReadDelimitedLine(char **cBuffer, const char *cDelimitedStr, unsigned int maxSize)
{
    std::cout << "ReadDelimitedLine 2" << std::endl;
    std::unique_ptr<char[]> atempStr(new char[maxSize]);
    long iBytesRead;

    std::cout << "FilePos: " << FilePos << std::endl;
    if (FilePos == FileSeek(0, std::ios::end)) // File Processed
        return 0;

    FileSeek(FilePos, std::ios::beg);
    iBytesRead = ReadLine(atempStr.get());
    std::cout << "atempStr.get() " << atempStr.get() << std::endl;
    std::cout << "bytes read " << iBytesRead << std::endl;
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
            std::cout << "FilePos: " << FilePos << std::endl;
            FilePos += 1;
            std::cout << "FilePos: " << FilePos << std::endl;
            continue;
        }
        else
            break;
    }
    if (cPos == nullptr)
    {
        cPos = cParamString + strlen(cParamString);
    }

    std::cout << "FilePos: " << FilePos << std::endl;
    FilePos += (cPos - cParamString);
    std::cout << "FilePos: " << FilePos << std::endl;

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

int CFileOps::ReadParamLine(char **cBuffer, unsigned int maxSize)
{
    return ReadDelimitedLine(cBuffer, "\t ", maxSize);
}

int CFileOps::ReadDelimitedLine(std::vector<std::string> &vecBuffer, const char *cDelimitedStr, unsigned int maxSize)
{
    std::cout << "ReadDelimitedLine 1" << std::endl;
    std::unique_ptr<char[]> aTempStr(new char[maxSize]);
    long iBytesRead;

    std::cout << "FilePos: " << FilePos << std::endl;
    if (FilePos == FileSeek(0, std::ios::end)) // File Processed
        return 0;


    FileSeek(FilePos, std::ios::beg);
    iBytesRead = ReadLine(aTempStr.get());
    std::cout << "aTempStr.get() " << aTempStr.get() << std::endl;
    std::cout << "bytes read " << iBytesRead << std::endl;
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
            std::cout << "FilePos: " << FilePos << std::endl;
            FilePos += 1;
            std::cout << "FilePos: " << FilePos << std::endl;
            continue;
        }
        else
            break;
    }
    if (cPos == nullptr)
    {
        cPos = cParamString + strlen(cParamString);
    }

    std::cout << "FilePos: " << FilePos << std::endl;
    FilePos += (cPos - cParamString);
    std::cout << "FilePos: " << FilePos << std::endl;
    cParamString[cPos - cParamString] = 0;

    //if (cParamString[0] == '#')
        //return -1;

    //for (int j = strlen(cParamString) - 1; j > 0 && cParamString[j] == ' '; j--)
        //cParamString[j] = 0;

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

int CFileOps::ReadParamLine(std::vector<std::string> &vecBuffer, unsigned int maxSize)
{
    return ReadDelimitedLine(vecBuffer, "\t ", maxSize);
}

int CFileOps::ReadFullLine(std::string &sOut)
{
    std::vector<std::string> vecBuffer;
    int rc = ReadDelimitedLine(vecBuffer, "\r\n", MAX_STRING_LENGTH * 4);

    if (!vecBuffer.empty())
        sOut = vecBuffer[0];

    return rc;
}
