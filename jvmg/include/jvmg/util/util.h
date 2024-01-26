//
// Created by Micky on 1/25/2024.
//

#ifndef _UTIL_H
#define _UTIL_H

#include <cstdint>
#include <fstream>
#include <vector>

namespace jvmg {
    class Serializable {
    public:
        std::vector<std::uint8_t> &serialize();

        void clearBuffer() { _buffer.clear(); }

        std::vector<uint8_t> &getBytes() { return _buffer; }

        void insertBytes(std::vector<std::uint8_t> &bytes);

        void serializeBytes(std::uint8_t bytes);

        // Example - takes in 0xCAFE and serializes [CA, FE]
        void serializeBytes(std::uint16_t bytes);

        // Example - takes in 0xCAFEBABE and serializes [CA, FE, BA, BE]
        void serializeBytes(std::uint32_t bytes);

        void outputToFile(const std::string& filename);

    private:
        virtual void _serialize() = 0;

        std::vector<std::uint8_t> _buffer;
    };
}

#endif //_UTIL_H
