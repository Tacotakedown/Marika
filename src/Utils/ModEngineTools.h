//
// Created by Taco on 7/9/2024.
//

#ifndef MARIKA_MODENGINETOOLS_H
#define MARIKA_MODENGINETOOLS_H

#include <string>
#include <iostream>
#include <filesystem>
#include "files/ModEngine2.h"


bool createDirectories(const std::string &path) {
    std::filesystem::path dirPath(path);
    if (std::filesystem::create_directories(dirPath.parent_path())) {
        return true;
    } else if (std::filesystem::exists(dirPath.parent_path())) {
        return true;
    }
    return false;
}

void extractZipFile(const unsigned char *zipData, size_t zipSize, const std::string &outputDir) {
    unzFile uf = unzOpenBuffer(zipData, zipSize);
    if (!uf) {
        std::cerr << "Failed to open zip from memory" << std::endl;
        return;
    }

    if (unzGoToFirstFile(uf) != UNZ_OK) {
        std::cerr << "Failed to go to first file in zip" << std::endl;
        unzClose(uf);
        return;
    }

    do {
        char filename[256];
        unz_file_info fileInfo;
        if (unzGetCurrentFileInfo(uf, &fileInfo, filename, sizeof(filename), NULL, 0, NULL, 0) != UNZ_OK) {
            std::cerr << "Failed to get file info" << std::endl;
            break;
        }

        if (unzOpenCurrentFile(uf) != UNZ_OK) {
            std::cerr << "Failed to open current file in zip" << std::endl;
            break;
        }

        std::string outputPath = outputDir + "/" + filename;
        if (filename[strlen(filename) - 1] == '/') {
            // Directory entry
            if (!createDirectories(outputPath)) {
                std::cerr << "Failed to create directory: " << outputPath << std::endl;
                unzCloseCurrentFile(uf);
                break;
            }
        } else {
            // File entry
            if (!createDirectories(outputPath)) {
                std::cerr << "Failed to create directories for: " << outputPath << std::endl;
                unzCloseCurrentFile(uf);
                break;
            }

            FILE *outputFile = fopen(outputPath.c_str(), "wb");
            if (!outputFile) {
                std::cerr << "Failed to open output file: " << outputPath << std::endl;
                unzCloseCurrentFile(uf);
                break;
            }

            char buffer[4096];
            int readBytes;
            while ((readBytes = unzReadCurrentFile(uf, buffer, sizeof(buffer))) > 0) {
                fwrite(buffer, 1, readBytes, outputFile);
            }

            fclose(outputFile);
        }

        unzCloseCurrentFile(uf);
    } while (unzGoToNextFile(uf) == UNZ_OK);

    unzClose(uf);
}

void unzipTestCase() {
    const std::string outDir = "C:\\test\\out";

    extractZipFile(g_ModEngine2, sizeof(g_ModEngine2), outDir);
}


#endif //MARIKA_MODENGINETOOLS_H
