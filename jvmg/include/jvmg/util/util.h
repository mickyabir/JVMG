//
// Created by Micky on 1/25/2024.
//

#ifndef _UTIL_H
#define _UTIL_H

#include <cstdint>
#include <vector>

class Serializable {
public:
    std::vector<std::uint8_t>& serialize() {
        clearBytes();
        _serialize();
        return getBytes();
    }

    std::vector<uint8_t>& getBytes() { return _bytes; }

    void clearBytes() { _bytes.clear(); }

    void insertBytes(std::vector<std::uint8_t> &bytes) {
        _bytes.insert(_bytes.end(), bytes.begin(), bytes.end());
    }

    void serializeBytes(std::uint8_t bytes) {
        _bytes.push_back(bytes);
    }

    // Example - takes in 0xCAFE and serializes [CA, FE]
    void serializeBytes(std::uint16_t bytes) {
        _bytes.push_back((std::uint8_t) ((bytes & 0xFF00) >> 8));
        _bytes.push_back((std::uint8_t) (bytes & 0xFF));
    }

    // Example - takes in 0xCAFEBABE and serializes [CA, FE, BA, BE]
    void serializeBytes(std::uint32_t bytes) {
        _bytes.push_back((std::uint8_t) ((bytes & 0xFF000000) >> 24));
        _bytes.push_back((std::uint8_t) ((bytes & 0xFF0000) >> 16));
        _bytes.push_back((std::uint8_t) ((bytes & 0xFF00) >> 8));
        _bytes.push_back((std::uint8_t) (bytes & 0xFF));
    }

private:
    virtual void _serialize() = 0;

    std::vector<std::uint8_t> _bytes;
};

#endif //_UTIL_H
