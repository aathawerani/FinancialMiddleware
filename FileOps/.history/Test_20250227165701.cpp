#include "FileOps.h"
#include <iostream>
#include <vector>
#include <string>
#include <system_error> // For system error codes
#include <cerrno>       // For errno

#include "../FileIO/Directory.h"

#define MAX_STRING_LENGTH 1024  // Ensure MAX_STRING_LENGTH is defined

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
            return 1;
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
        file.WriteParamLine("ParamA, %s, %s", "ParamB", "ParamC");

        // Ensure data is written to disk
        file.flush();

        // Go to beginning of file
        file.FileSeek(0, std::ios::beg);

        // Read full line
        std::string fullLine;
        file.ReadLine(fullLine);
        std::cout << "Read full line: " << fullLine << std::endl;

        // Read delimited line into vector
        std::vector<std::string> vecBuffer;
        int paramsRead = file.ReadDelimitedLine(vecBuffer, ",", MAX_STRING_LENGTH);
        std::cout << "Read delimited line into vector: " << paramsRead << " parameters" << std::endl;
        for (int i = 0; i < paramsRead; ++i) {
            std::cout << "Param " << i << ": " << vecBuffer[i] << std::endl;
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


        std::vector<unsigned char> sampleData = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0xC2, 0xA9, 0x21, 0x00};

        const char* filename = "example.bin";  // Change this to your actual file
        CFileOps file(filename, std::ios::binary);
        file.WriteLine(reinterpret_cast<const char*>(sampleData.data()));
        file.close();
        std::cout << "Binary file '" << filename << "' generated successfully!\n";

        std::vector<unsigned char> fileData = CFileOps::ReadBinary(filename);
        std::cout << "Successfully read " << fileData.size() << " bytes from file: " << filename << std::endl;

        // Print first few bytes (for demonstration)
        std::cout << "File Content (first 10 bytes): ";
        for (size_t i = 0; i < std::min<size_t>(10, fileData.size()); ++i) {
            std::cout << std::hex << static_cast<int>(fileData[i]) << " ";
        }
        std::cout << std::dec << std::endl;  // Reset back to decimal

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
