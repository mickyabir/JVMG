//
// Created by Micky on 1/25/2024.
//

#include "jvmg/util/util.h"

using namespace jvmg;

std::vector<std::uint8_t>& Serializable::serialize() {
    clearBuffer();
    _serialize();
    return getBytes();
}

void Serializable::insertBytes(std::vector<std::uint8_t> &bytes) {
    _buffer.insert(_buffer.end(), bytes.begin(), bytes.end());
}

void Serializable::serializeBytes(std::uint8_t bytes) {
    _buffer.push_back(bytes);
}

void Serializable::serializeBytes(std::uint16_t bytes) {
    _buffer.push_back((std::uint8_t) ((bytes & 0xFF00) >> 8));
    _buffer.push_back((std::uint8_t) (bytes & 0xFF));
}

void Serializable::serializeBytes(std::uint32_t bytes) {
    _buffer.push_back((std::uint8_t) ((bytes & 0xFF000000) >> 24));
    _buffer.push_back((std::uint8_t) ((bytes & 0xFF0000) >> 16));
    _buffer.push_back((std::uint8_t) ((bytes & 0xFF00) >> 8));
    _buffer.push_back((std::uint8_t) (bytes & 0xFF));
}

void Serializable::outputToFile(std::string_view filename) {
    std::ofstream outputStream(filename.data(), std::ios::out | std::ios::binary);

    if (_buffer.empty()) {
        _serialize();
    }

    for (auto byte : _buffer) {
        outputStream.write((char *) &byte, sizeof(std::uint8_t));
    }
}