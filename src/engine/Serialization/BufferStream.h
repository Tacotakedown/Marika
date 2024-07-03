//
// Created by Taco on 7/2/2024.
//

#ifndef BUFFERSTREAM_H
#define BUFFERSTREAM_H

#include "StreamReader.h"
#include "StreamWriter.h"

#include <filesystem>
#include <fstream>

namespace Marika {
    class BufferStreamWriter : public StreamWriter {
    public:
        BufferStreamWriter(Buffer targetBuffer, uint64_t position = 0);

        BufferStreamWriter(const BufferStreamWriter &) = delete;

        virtual ~BufferStreamWriter() override = default;

        bool IsStreamGood() const final { return (bool) m_TargetBuffer; }
        uint64_t GetStreamPosition() override { return m_BufferPosition; }
        void SetStreamPosition(uint64_t position) override { m_BufferPosition = position; }

        bool WriteData(const char *data, size_t size) final;

        Buffer GetBuffer() const { return Buffer(m_TargetBuffer, m_BufferPosition); }

    private:
        Buffer m_TargetBuffer;
        uint64_t m_BufferPosition = 0;
    };


    class BufferStreamReader : public StreamReader {
    public:
        BufferStreamReader(Buffer targetBuffer, uint64_t position = 0);

        BufferStreamReader(const BufferStreamReader &) = delete;

        virtual ~BufferStreamReader() override = default;

        bool IsStreamGood() const final { return (bool) m_TargetBuffer; }
        uint64_t GetStreamPosition() override { return m_BufferPosition; }
        void SetStreamPosition(uint64_t position) override { m_BufferPosition = position; }

        bool ReadData(char *destination, size_t size) override;

        Buffer GetBuffer() const { return Buffer(m_TargetBuffer, m_BufferPosition); }

    private:
        Buffer m_TargetBuffer;
        uint64_t m_BufferPosition = 0;
    };
}

#endif //BUFFERSTREAM_H
