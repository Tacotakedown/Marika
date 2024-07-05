//
// Created by Taco on 7/2/2024.
//

#ifndef DLLWRITER_H
#define DLLWRITER_H

#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

bool CreateDllFromMemory(const std::wstring &baseFolderPath, const std::wstring &fileName, const unsigned char *data, size_t size) {
    try {
        std::wstring filePath = baseFolderPath + L"\\" + fileName;

        std::ofstream outFile(filePath, std::ios::binary);
        if (!outFile) {
            std::wcerr << L"Error opening file: " << filePath << std::endl;
            return false;
        }

        outFile.write(reinterpret_cast<const char *>(data), size);

        if (outFile.good()) {
            return true;
        } else {
            std::wcerr << L"Error writing data to file: " << filePath << std::endl;
            return false;
        }
    } catch (const std::exception &e) {
        std::wcerr << L"Exception caught: " << e.what() << std::endl;
        return false;
    }
}

bool RenameSteamDll(const std::wstring &basePath, const std::wstring &oldFileName, const std::wstring &newFileName) {
    try {
        fs::path oldFilePath = fs::path(basePath) / oldFileName;
        fs::path newFilePath = fs::path(basePath) / newFileName;

        fs::rename(oldFilePath, newFilePath);
        return true;
    } catch (const fs::filesystem_error &e) {
        std::wcerr << L"Error renaming file: " << e.what() << std::endl;
        return false;
    }
}

bool DeleteDllFile(const std::wstring &basePath, const std::wstring &fileName) {
    try {
        fs::path filePath = fs::path(basePath) / fileName;

        if (fs::remove(filePath)) {
            return true;
        } else {
            std::wcerr << L"File not found: " << filePath << std::endl;
            return false;
        }
    } catch (const fs::filesystem_error &e) {
        std::wcerr << L"Error deleting file: " << e.what() << std::endl;
        return false;
    }
}

#endif //DLLWRITER_H
