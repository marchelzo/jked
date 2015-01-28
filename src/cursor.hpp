#include "buffer.hpp"

#pragma once

class Cursor {
    size_t row;
    size_t col;
    const Buffer& buffer;
public:
    Cursor(const Buffer& buffer, size_t row, size_t col);
    bool up();
    bool down();
    bool left();
    bool right();
    size_t get_row();
    size_t get_col();
};
