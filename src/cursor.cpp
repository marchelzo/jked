#include "cursor.hpp"

Cursor::Cursor(const Buffer& buffer, size_t row, size_t col):
    buffer {buffer},
    row {row},
    col {col}
{ }

bool Cursor::up()
{
    if (row == 1) return false;
    return --row, true;
}

bool Cursor::down()
{
    if (buffer.get_num_lines() == row) return false;
    return ++row, true;
}

bool Cursor::left()
{
    if (col == 1) return false;
    return --col, true;
}

bool Cursor::right()
{
    if (buffer.get_line(row).length() == col) return false;
    return ++col, true;
}

size_t Cursor::get_row()
{
    return row;
}

size_t Cursor::get_col()
{
    return col;
}
