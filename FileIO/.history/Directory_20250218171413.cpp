#include "directory.h"
#include <filesystem>
#include <string>

CDirectory::CDirectory(void)
{
}

CDirectory::~CDirectory(void)
{
}

/**
* Does Folder Exist?
* Checks whether the given folder exists in the file system or not
* Parameter: 
*	cDirName	fully resolved directory name
*/
bool CDirectory::doesFolderExist (const char *cFolderName)
{
    return std::filesystem::is_directory(cFolderName);
}

/**
* Does File Exist?
* Checks whether the given file exists in the file system or not
* Parameter
*	cFilename	fully resolved filename
*/
bool CDirectory::doesFileExist (const char *cFilename)
{
    return std::filesystem::exists(cFilename);
}

/**
* Create Folder
* Given folder name is created
* Returns false if failed to create the folder for any reason
*/
bool CDirectory::createFolder (const char *cFolderName)
{
    return std::filesystem::create_directory(cFolderName);
}

/**
* Get Temp Path
* Returns the path to temporary folder
*/
std::string CDirectory::getTempPath ()
{
    return std::filesystem::temp_directory_path().string();
}

/**
* Break Filename
* Breaks a complete filename into filename and extension
*/
bool CDirectory::breakFilename (const char *cFilenameWithExt, char *cFilename, char *cExtension)
{
    std::filesystem::path p(cFilenameWithExt);
    std::string filename = p.stem().string();
    std::string extension = p.extension().string();
    strcpy(cFilename, filename.c_str());
    strcpy(cExtension, extension.c_str());
    return true;
}

/**
* Split On Character
* Breaks a string based on the split character passed (finds in reverse order)
*/
bool CDirectory::splitOnChar (const char *cSource, const char delim, char *cPartBeforeDelim, char *cPartAfterDelim)
{
    std::string source(cSource);
    size_t pos = source.rfind(delim);
    if (pos != std::string::npos) 
    {
        std::string beforeDelim = source.substr(0, pos);
        std::string afterDelim = source.substr(pos + 1);
        strcpy(cPartBeforeDelim, beforeDelim.c_str());
        strcpy(cPartAfterDelim, afterDelim.c_str());
    } 
    else 
    {
        strcpy(cPartBeforeDelim, cSource);
        strcpy(cPartAfterDelim, "");
    }
    return true;
}

/**
* Break Qualified Name
* Breaks a fully qualified name into path and filename
*/
bool CDirectory::breakQualifiedName (const char *cQualifiedName, char *cPath, char *cFilename)
{
    std::filesystem::path p(cQualifiedName);
    std::string path = p.parent_path().string();
    std::string filename = p.filename().string();
    strcpy(cPath, path.c_str());
    strcpy(cFilename, filename.c_str());
    return true;
}

/**
* Move File
* Moves given file from the source folder to destination folder
* Deletes destination file if it already exists
*/
bool CDirectory::moveFile (const char *cDestFolder, const char *cSrcFolder, const char *cFilename)
{
    std::filesystem::path srcPath = std::filesystem::path(cSrcFolder) / cFilename;
    std::filesystem::path destPath = std::filesystem::path(cDestFolder) / cFilename;
    std::filesystem::remove(destPath);
    std::filesystem::rename(srcPath, destPath);
    return true;
}