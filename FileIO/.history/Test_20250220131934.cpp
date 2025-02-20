#include "FileIO.h"
#include "directory.h"
#include <iostream>
#include <vector>
#include <string>
#include <system_error> // Include for system error codes
#include <cerrno> // Include for errno

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

        // Write multiple lines to the file
        const char* testLine1 = "This is the first test line";
        const char* testLine2 = "This is the second test line";
        const char* testLine3 = "This is the third test line";
        file.WriteLine(testLine1, strlen(testLine1));
        file.WriteLine(testLine2, strlen(testLine2));
        file.WriteLine(testLine3, strlen(testLine3));

        // Ensure data is written to disk
        file.flush();

        // Read multiple lines from the file
        char buffer[MAX_STRING_LENGTH];
        file.FileSeek(0, std::ios::beg);
        int bytesRead = file.ReadLine(buffer);
        std::cout << "Read from file: " << buffer << std::endl;
        std::cout << "bytesRead: " << bytesRead << std::endl;
        bytesRead = file.ReadLine(buffer);
        std::cout << "Read from file: " << buffer << std::endl;
        std::cout << "bytesRead: " << bytesRead << std::endl;
        bytesRead = file.ReadLine(buffer);
        std::cout << "Read from file: " << buffer << std::endl;
        std::cout << "bytesRead: " << bytesRead << std::endl;

        // Break filename
        char filename[MAX_STRING_LENGTH];
        char extension[MAX_STRING_LENGTH];
        dir.breakFilename(fileName, filename, extension);
        std::cout << "Filename: " << filename << ", Extension: " << extension << std::endl;

        // Break qualified name
        char path[MAX_STRING_LENGTH];
        char fname[MAX_STRING_LENGTH];
        dir.breakQualifiedName(fileName, path, fname);
        std::cout << "Path: " << path << ", Filename: " << fname << std::endl;

        // Close the file before moving it
        file.close();

        // Move file
        const char* destFolder = "testFolder2";
        if (!dir.doesFolderExist(destFolder)) {
            dir.createFolder(destFolder);
        }
        if (dir.moveFile(destFolder, folderName, "testFile.txt")) {
            std::cout << "File moved to: " << destFolder << std::endl;
        } else {
            std::cerr << "Failed to move file to: " << destFolder << std::endl;
        }

        // Get temp path
        std::string tempPath = dir.getTempPath();
        std::cout << "Temp path: " << tempPath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
