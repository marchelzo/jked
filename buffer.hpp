#pragma once

#include <string>
#include <vector>
#include <memory>

class Transformation;
class Buffer;

class Insert;
class Delete;
class NewLine;
class DeleteLine;

class Transformation {
public:
    virtual std::unique_ptr<Transformation> inverted() const = 0;
    virtual void apply(Buffer&) const = 0;
};

class Insert : public Transformation {
    const std::string s;
    size_t row;
    size_t col;
public:
    std::unique_ptr<Transformation> inverted() const;
    void apply(Buffer&) const;
    Insert(std::string, size_t, size_t);
};

class Delete : public Transformation {
    const std::string s;
    size_t row;
    size_t col;
public:
    std::unique_ptr<Transformation> inverted() const;
    void apply(Buffer&) const;
    Delete(std::string, size_t, size_t);
};

class NewLine : public Transformation {
public:
    std::unique_ptr<Transformation> inverted() const;
    void apply(Buffer&) const;
};

class DeleteLine : public Transformation {
public:
    std::unique_ptr<Transformation> inverted() const;
    void apply(Buffer&) const;
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

    bool go_to_line(size_t row);

public:
    Buffer();
    bool insert(size_t row, size_t col, const std::string& s);
    bool erase(size_t row, size_t col, const std::string& s);
    bool new_line(size_t row);
    bool delete_line(size_t row);
};
