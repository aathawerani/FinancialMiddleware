#include "FileOps.h"
#include <iostream>
#include <vector>
#include <string>
#include <system_error> // Include for system error codes
#include <cerrno> // Include for errno

#include "../FileIO/Directory.h"

int main() {
    // Directory operations
    CDirectory dir;
    const char* folderName = "testFolder";
    const char* fileName = "testFolder/testFile.txt";

    // Create folder
    if (!dir.doesFolderExist(folderName)) {
        if (dir.createFolder(folderName)) {
            std::cout << "Folder created: " << folderName << std::endl;
        } else {
            std::cerr << "Failed to create folder: " << folderName << std::endl;
            return 1; // Exit if folder creation fails
        }
    } else {
        std::cout << "Folder already exists: " << folderName << std::endl;
    }

    // File operations
    try {
        std::cout << "Attempting to create file: " << fileName << std::endl;
        CFileIO file(fileName, CFileIO::FLAG_CREATE_ALWAYS | std::ios::in | std::ios::out);
        std::cout << "File created successfully: " << fileName << std::endl;

        // Write to file
        file.WriteParamLine("This is a test line: %d", 1);

        // Ensure data is written to disk
        file.flush();

        // Read from file
        char buffer[MAX_STRING_LENGTH];
        file.FileSeek(0, std::ios::beg);
        file.ReadLine(buffer);
        std::cout << "Read from file: " << buffer << std::endl;

        // Close the file before moving it
        file.close();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
