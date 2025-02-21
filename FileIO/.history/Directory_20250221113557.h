#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>

class CDirectory {
public:
    CDirectory();
    ~CDirectory();

    bool doesFolderExist(const std::string& folderName);
    bool doesFileExist(const std::string& filename);
    bool createFolder(const std::string& folderName);
    std::string getTempPath();
    std::pair<std::string, std::string> breakFilename(const std::string& filenameWithExt);
    std::pair<std::string, std::string> splitOnChar(const std::string& source, char delim);
    std::pair<std::string, std::string> breakQualifiedName(const std::string& qualifiedName);
    bool moveFile(const std::string& destFolder, const std::string& srcFolder, const std::string& filename);
};

#endif // DIRECTORY_H
