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
        CFileOps file(fileName, CFileIO::FLAG_CREATE_ALWAYS | std::ios::in | std::ios::out);
        std::cout << "File created successfully: " << fileName << std::endl;

        // Write to file
        file.WriteParamLine("This is a test line: %d", 1);
        file.WriteParamLine("Param1,Param2,Param3");
        file.WriteParamLine("ParamA, %s, %s","ParamB","ParamC");

        // Ensure data is written to disk
        file.flush();

        // Read multiple lines from the file
        char buffer1[MAX_STRING_LENGTH];
        file.FileSeek(0, std::ios::beg);
        int bytesRead = file.ReadLine(buffer1);
        std::cout << "Read from file: " << buffer1 << std::endl;
        std::cout << "bytesRead: " << bytesRead << std::endl;
        bytesRead = file.ReadLine(buffer1);
        std::cout << "Read from file: " << buffer1 << std::endl;
        std::cout << "bytesRead: " << bytesRead << std::endl;
        bytesRead = file.ReadLine(buffer1);
        std::cout << "Read from file: " << buffer1 << std::endl;
        std::cout << "bytesRead: " << bytesRead << std::endl;

        // Read from file
        char buffer[MAX_STRING_LENGTH];
        file.clear();  // Clear fail and EOF flags
        file.FileSeek(0, std::ios::beg);
        file.ReadLine(buffer);
        std::cout << "Read from file: " << buffer << std::endl;

        // Read full line
        std::string fullLine;
        file.ReadFullLine(fullLine);
        std::cout << "Read full line: " << fullLine << std::endl;

        // Read delimited line into char buffer
        char* cBuffer[10];
        for (int i = 0; i < 10; ++i) {
            cBuffer[i] = new char[MAX_STRING_LENGTH];
        }
        int paramsRead = file.ReadDelimitedLine(cBuffer, ",", MAX_STRING_LENGTH);
        std::cout << "Read delimited line into char buffer: " << paramsRead << " parameters" << std::endl;
        for (int i = 0; i < paramsRead; ++i) {
            std::cout << "Param " << i << ": " << cBuffer[i] << std::endl;
            delete[] cBuffer[i];
        }

        // Read delimited line into vector
        std::vector<std::string> vecBuffer;
        paramsRead = file.ReadDelimitedLine(vecBuffer, ",", MAX_STRING_LENGTH);
        std::cout << "Read delimited line into vector: " << paramsRead << " parameters" << std::endl;
        for (int i = 0; i < paramsRead; ++i) {
            std::cout << "Param " << i << ": " << vecBuffer[i] << std::endl;
        }

        // Read param line into char buffer
        for (int i = 0; i < 10; ++i) {
            cBuffer[i] = new char[MAX_STRING_LENGTH];
        }
        paramsRead = file.ReadParamLine(cBuffer, MAX_STRING_LENGTH);
        std::cout << "Read param line into char buffer: " << paramsRead << " parameters" << std::endl;
        for (int i = 0; i < paramsRead; ++i) {
            std::cout << "Param " << i << ": " << cBuffer[i] << std::endl;
            delete[] cBuffer[i];
        }

        // Read param line into vector
        vecBuffer.clear();
        paramsRead = file.ReadParamLine(vecBuffer, MAX_STRING_LENGTH);
        std::cout << "Read param line into vector: " << paramsRead << " parameters" << std::endl;
        for (int i = 0; i < paramsRead; ++i) {
            std::cout << "Param " << i << ": " << vecBuffer[i] << std::endl;
        }

        // Close the file before moving it
        file.close();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
