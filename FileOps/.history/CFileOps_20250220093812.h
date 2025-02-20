#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include <mutex>
#include <vector>
#include <string>

class CFileOps {
public:
    CFileOps(const char *cFileName, int iFlag);
    ~CFileOps(void);

    int WriteParamLine(const char* cFormat, ...);
    int ReadDelimitedLine(char **cBuffer, const char *cDelimitedStr, unsigned int maxSize);
    int ReadParamLine(char **cBuffer, unsigned int maxSize);
    int ReadDelimitedLine(std::vector<std::string> &vecBuffer, const char *cDelimitedStr, unsigned int maxSize);
    int ReadParamLine(std::vector<std::string> &vecBuffer, unsigned int maxSize);
    int ReadFullLine(std::string &sOut);


};

#endif // FILEIO_H