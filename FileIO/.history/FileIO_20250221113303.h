#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include <mutex>
#include <string>
#include <string_view>

class CFileIO {
public:
    static const int FLAG_CREATE_NEW;
    static const int FLAG_CREATE_ALWAYS;
    static const int FLAG_OPEN_EXISTING;
    static const int FLAG_OPEN_ALWAYS;
    static const int FLAG_TRUNCATE_EXISTING;

    CFileIO(std::string fileName, int flags);
    ~CFileIO() = default;

    std::streampos FileSeek(std::streamoff distance, std::ios_base::seekdir direction);
    long WriteLine(const std::string& buffer);
    long ReadLine(std::string& buffer);

    void flush();
    void close();
    void clear();
    std::streampos getFpos() ;

protected:
    mutable std::mutex fileMutex; // Use `mutable` for locking in const methods
    std::fstream fileStream;
    std::string cfileName;
};

#endif // FILEIO_H
