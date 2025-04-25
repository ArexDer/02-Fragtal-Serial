#pragma once

#define PALETTE_SIZE 16

#include <cstdint>
#include <vector>
//INVIERTE LOS BYTES
uint32_t _bswap32(uint32_t a) {
    return
        ((a & 0X000000FF) << 24) |
        ((a & 0X0000FF00) <<  8) |
        ((a & 0x00FF0000) >>  8) |
        ((a & 0xFF000000) >> 24);
}

std::vector<unsigned int> color_ramp = {
    _bswap32(0xA8E6A1FF),  // Verde claro
    _bswap32(0xA0E0A1FF),
    _bswap32(0x98DA9CFF),
    _bswap32(0x90D493FF),
    _bswap32(0x88CD8EFF),
    _bswap32(0x80C689FF),
    _bswap32(0x78BF83FF),
    _bswap32(0x70B57EFF),
    _bswap32(0x69AA79FF),
    _bswap32(0x618F72FF),
    _bswap32(0x59766CFF),
    _bswap32(0x4F5C65FF),
    _bswap32(0x474C5FFF),
    _bswap32(0x3F3B58FF),
    _bswap32(0x37264FFF),
    _bswap32(0xFF6347FF)   // Tomate claro
};
