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
        file.WriteLine("This is the first test line");
        file.WriteLine("This is the second test line");
        file.WriteLine("This is the third test line");

        // Ensure data is written to disk
        file.flush();

        // Read multiple lines from the file
        file.FileSeek(0, std::ios::beg);

        std::string line;
        while (file.ReadLine(line) > 0) {
            std::cout << "Read line: " << line << std::endl;
        }

        // Break filename
        std::string filename, extension;
        std::tie(filename, extension) = dir.breakFilename(fileName);

        std::cout << "Filename: " << filename << ", Extension: " << extension << std::endl;

        // Break qualified name
        auto [path, fname] = dir.breakQualifiedName(fileName);
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


        std::vector<unsigned char> sampleData = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0xC2, 0xA9, 0x21, 0x00};

        const char* binfilename = "example.bin";  // Change this to your actual file
        CFileIO binfile(binfilename, std::ios::binary  | std::ios::out);
        binfile.WriteLine(sampleData);
        binfile.flush();
        binfile.close();
        std::cout << "Binary file '" << binfilename << "' generated successfully!\n";


    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
