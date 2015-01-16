#pragma once

#include <string>
#include <vector>

class Transformation;
class Buffer;

class Transformation {
public:
    virtual Transformation inverted() const = 0;
    virtual void apply(Buffer&) = 0;
};

class Insert : public Transformation {
    const std::string s;
    size_t row;
    size_t col;
public:
    virtual Delete inverted() const;
    virtual void apply(Buffer&) const;
    Insert(std::string, size_t, size_t);
};

class Delete : public Transformation {
    const std::string s;
    size_t row;
    size_t col;
public:
    virtual Insert inverted() const;
    virtual void apply(Buffer&) const;
    Delete(std::string, size_t, size_t);
};

class NewLine : public Transformation {
public:
    virtual DeleteLine inverted() const;
    virtual void apply(Buffer&) const;
};

class DeleteLine : public Transformation {
public:
    virtual InsertLine inverted() const;
    virtual void apply(Buffer&) const;
};

struct Line {
    Line* prev;
    std::string content;
    Line* next;
};

class Buffer {
    Line *first;
    Line *current;
    Line *last;
    size_t current_line;
    size_t num_lines;
    std::vector<Transformation> history;
public:
    Buffer();
};
