#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>

class CDirectory {
public:
    CDirectory();
    ~CDirectory();

    bool doesFolderExist(const char *cFolderName);
    bool doesFileExist(const char *cFilename);
    bool createFolder(const char *cFolderName);
    std::string getTempPath();
    bool breakFilename(const char *cFilenameWithExt, char *cFilename, char *cExtension);
    bool splitOnChar(const char *cSource, const char delim, char *cPartBeforeDelim, char *cPartAfterDelim);
    bool breakQualifiedName(const char *cQualifiedName, char *cPath, char *cFilename);
    bool moveFile(const char *cDestFolder, const char *cSrcFolder, const char *cFilename);
};

#endif // DIRECTORY_H
