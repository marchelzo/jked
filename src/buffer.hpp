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
    virtual bool apply(Buffer&) const = 0;
};

class Insert : public Transformation {
    const std::string s;
    size_t row;
    size_t col;
public:
    std::unique_ptr<Transformation> inverted() const;
    bool apply(Buffer&) const;
    Insert(std::string, size_t, size_t);
};

class Delete : public Transformation {
    const std::string s;
    size_t row;
    size_t col;
public:
    std::unique_ptr<Transformation> inverted() const;
    bool apply(Buffer&) const;
    Delete(std::string, size_t, size_t);
};

class NewLine : public Transformation {
    size_t row;
public:
    std::unique_ptr<Transformation> inverted() const;
    bool apply(Buffer&) const;
    NewLine(size_t row);
};

class DeleteLine : public Transformation {
    size_t row;
public:
    std::unique_ptr<Transformation> inverted() const;
    bool apply(Buffer&) const;
    DeleteLine(size_t row);
};

struct Line {
    Line* prev;
    std::string content;
    Line* next;
    Line();
};

class Buffer {

friend class Insert;
friend class Delete;
friend class NewLine;
friend class DeleteLine;

private:
    Line* first;
    Line* current;
    Line* last;
    size_t current_line;
    size_t num_lines;
    std::vector<std::unique_ptr<Transformation>> history;
    int history_index;

    bool go_to_line(size_t row);
    bool insert(size_t row, size_t col, const std::string& s);
    bool erase(size_t row, size_t col, size_t len);
    bool new_line(size_t row);
    bool delete_line(size_t row);
public:
    Buffer();
    bool transform(Transformation* t);
    bool undo();
    bool redo();
    void draw();
};
