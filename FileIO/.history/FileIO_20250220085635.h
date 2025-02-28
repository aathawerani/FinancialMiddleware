#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include <mutex>
#include <vector>
#include <string>

const int MAX_FILE_LENGTH = 1024;
const int MAX_STRING_LENGTH = 1024;

class CFileIO {
public:
    static const int FLAG_CREATE_NEW;
    static const int FLAG_CREATE_ALWAYS;
    static const int FLAG_OPEN_EXISTING;
    static const int FLAG_OPEN_ALWAYS;
    static const int FLAG_TRUNCATE_EXISTING;

    CFileIO(const char *cFileName, int iFlag);
    ~CFileIO(void);

    std::streampos FileSeek(std::streamoff distance, std::ios_base::seekdir direction);
    long WriteLine(const char *cBuffer, int iBufferLen);
    long ReadLine(char *cBuffer);
    int WriteParamLine(const char* cFormat, ...);
    int ReadDelimitedLine(char **cBuffer, const char *cDelimitedStr, unsigned int maxSize);
    int ReadParamLine(char **cBuffer, unsigned int maxSize);
    int ReadDelimitedLine(std::vector<std::string> &vecBuffer, const char *cDelimitedStr, unsigned int maxSize);
    int ReadParamLine(std::vector<std::string> &vecBuffer, unsigned int maxSize);
    int ReadFullLine(std::string &sOut);

    // Add the flush function declaration
    void flush();

private:
    std::fstream fileStream;
    std::string cFileName;
    std::mutex fileMutex;
    std::streampos FilePos;
};

#endif // FILEIO_H