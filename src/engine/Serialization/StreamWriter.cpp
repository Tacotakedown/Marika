//
// Created by Taco on 7/2/2024.
//

#include "StreamWriter.h"

namespace ModLauncher {
    void StreamWriter::WriteBuffer(Buffer buffer, bool writeSize) {
        if (writeSize)
            WriteData(static_cast<char *>(&buffer.Size()), sizeof(uint32_t));

        WriteData(static_cast<char *>(buffer.Data), buffer.Size);
    }

    void StreamWriter::WriteZero(uint64_t size) {
        char zero = 0;
        for (uint64_t i = 0; i < size; i++) {
            WriteData(&zero, 1);
        }
    }

    void StreamWriter::WriteString(const std::string &string) {
        size_t size = string.size();
        WriteData(reinterpret_cast<char *>(&size), sizeof(size_t));
        WriteData(const_cast<char *>(string.data()), sizeof(char) * string.size()); // please dont break
    }

    void StreamWriter::WriteString(std::string_view string) {
        size_t size = string.size();
        WriteData(reinterpret_cast<char *>(&size), sizeof(size_t));
        WriteData(const_cast<char *>(string.data()), sizeof(char) * string.size()); // please dont break
    }
}
