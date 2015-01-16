#include "buffer.hpp"

Buffer::Buffer():
    current {new Line},
    current_line {1},
    num_lines    {1}
{
    first = current;
    last  = current;
}

Insert::Insert(std::string s, size_t row, size_t col):
    s {std::move(s)},
    row {row},
    col {col}
{
}

Delete::Delete(std::string s, size_t row, size_t col):
    s {std::move(s)},
    row {row},
    col {col}
{
}

Delete Insert::inverted() const
{
    return Delete {s, row, col};
}

Insert Delete::inverted() const
{
    return Insert {s, row, col};
}

DeleteLine NewLine::inverted() const
{
    return DeleteLine {};
}

NewLine DeleteLine::inverted() const
{
    return NewLine {};
}

void Insert::apply(Buffer& b) const
{
    
}

void Delete::apply(Buffer& b) const
{
    
}

void NewLine::apply(Buffer& b) const
{
    
}

void DeleteLine::apply(Buffer& b) const
{
    
}

int main()
{

}
