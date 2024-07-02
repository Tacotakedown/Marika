//
// Created by Taco on 7/2/2024.
//

#ifndef DLLWRITER_H
#define DLLWRITER_H

#include <iostream>
#include <fstream>

bool createDllFromMemory(const char *fileName, const unsigned char *data, size_t size) {
    std::ofstream outFile(fileName, std::ios::binary);
    if (!outFile) {
        std::cout << "Error opening file" << std::endl;
        return false;
    }
    outFile.write(reinterpret_cast<const char *>(data), size);
    return outFile.good();
}


#endif //DLLWRITER_H
