//
// Created by Taco on 7/2/2024.
//

#include "StreamReader.h"

namespace ModLauncher {
    bool StreamReader::ReadBuffer(Buffer &buffer, const uint32_t size) {
        buffer.Size = size;
        if (size == 0) {
            if (!ReadData(reinterpret_cast<char *>(&buffer.Size), sizeof(uint32_t)))
                return false;
        }

        buffer.Allocate(buffer.Size);
        return ReadData(static_cast<char *>(buffer.Data), buffer.Size);
    }

    bool StreamReader::ReadString(std::string &string) {
        size_t size;
        if (!ReadData(reinterpret_cast<char *>(&size), sizeof(size_t)))
            return false;

        string.resize(size);
        return ReadData(static_cast<char *>(string.data()), sizeof(char) * size);
    }
}