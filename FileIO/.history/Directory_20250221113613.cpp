#include "directory.h"
#include <filesystem>
#include <string>

CDirectory::CDirectory() = default;
CDirectory::~CDirectory() = default;

/**
 * Check if a folder exists.
 */
bool CDirectory::doesFolderExist(const std::string& folderName) {
    return std::filesystem::is_directory(folderName);
}

/**
 * Check if a file exists.
 */
bool CDirectory::doesFileExist(const std::string& filename) {
    return std::filesystem::exists(filename);
}

/**
 * Create a folder.
 */
bool CDirectory::createFolder(const std::string& folderName) {
    return std::filesystem::create_directory(folderName);
}

/**
 * Get temporary path.
 */
std::string CDirectory::getTempPath() {
    return std::filesystem::temp_directory_path().string();
}

/**
 * Break a filename into its base name and extension.
 */
std::pair<std::string, std::string> CDirectory::breakFilename(const std::string& filenameWithExt) {
    std::filesystem::path p(filenameWithExt);
    return { p.stem().string(), p.extension().string() };
}

/**
 * Split a string at the last occurrence of a character.
 */
std::pair<std::string, std::string> CDirectory::splitOnChar(const std::string& source, char delim) {
    size_t pos = source.rfind(delim);
    if (pos != std::string::npos) {
        return { source.substr(0, pos), source.substr(pos + 1) };
    }
    return { source, "" };
}

/**
 * Break a fully qualified path into directory path and filename.
 */
std::pair<std::string, std::string> CDirectory::breakQualifiedName(const std::string& qualifiedName) {
    std::filesystem::path p(qualifiedName);
    return { p.parent_path().string(), p.filename().string() };
}

/**
 * Move a file from the source folder to the destination folder.
 * Deletes destination file if it already exists.
 */
bool CDirectory::moveFile(const std::string& destFolder, const std::string& srcFolder, const std::string& filename) {
    std::filesystem::path srcPath = std::filesystem::path(srcFolder) / filename;
    std::filesystem::path destPath = std::filesystem::path(destFolder) / filename;
    
    if (std::filesystem::exists(destPath)) {
        std::filesystem::remove(destPath);
    }
    
    std::filesystem::rename(srcPath, destPath);
    return true;
}
