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

    // Add the flush and close function declarations
    void flush();
    void close();
    void clear();
    std::streampos getFpos();

protected:
    std::fstream fileStream;
    std::string cFileName;
    std::mutex fileMutex;
    std::streampos FilePos;
};

#endif // FILEIO_H