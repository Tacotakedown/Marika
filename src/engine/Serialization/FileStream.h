//
// Created by Taco on 7/2/2024.
//

#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "StreamReader.h"
#include "StreamWriter.h"

#include <filesystem>
#include <fstream>

namespace ModLauncher {
    class FileStreamWriter : public StreamWriter {
    public:
        FileStreamWriter(const std::filesystem::path &path);

        FileStreamWriter(const FileStreamWriter &) = delete;

        virtual ~FileStreamWriter();

        bool IsStreamGood() const final { return m_Stream.good(); }
        uint64_t GetStreamPosition() final { return m_Stream.tellp(); }
        void SetStreamPosition(uint64_t position) final { m_Stream.seekp(position); }

        bool WriteData(const char *data, size_t size) final;

    private:
        std::filesystem::path m_Path;
        std::ofstream m_Stream;
    };

    class FileStreamReader : public StreamReader {
    public:
        FileStreamReader(const std::filesystem::path &path);

        FileStreamReader(const FileStreamReader &) = delete;

        ~FileStreamReader();

        bool IsStreamGood() const final { return m_Stream.good(); }
        uint64_t GetStreamPosition() final { return m_Stream.tellp(); }
        void SetStreamPosition(uint64_t position) final { m_Stream.seekp(position); }

        bool ReadData(char *destination, size_t size) override;

    private:
        std::filesystem::path m_Path;
        std::ifstream m_Stream;
    };
}

#endif //FILESTREAM_H
