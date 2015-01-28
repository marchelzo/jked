#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>

class Transformation;
class Buffer;

class Insert;
class Delete;
class NewLine;
class DeleteLine;

#pragma once

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

class Line {
    SDL_Texture* texture;
    bool modified;
    void update_texture();
public:
    SDL_Rect rect;
    Line* prev;
    std::string content;
    Line* next;
    Line();
    SDL_Texture* get_texture();
    void modify();
};

class Buffer {

friend class Insert;
friend class Delete;
friend class NewLine;
friend class DeleteLine;

private:
    mutable Line* first;
    mutable Line* current;
    mutable Line* last;
    mutable size_t current_line;
    size_t num_lines;
    std::vector<std::unique_ptr<Transformation>> history;
    int history_index;

    bool go_to_line(size_t row) const;
    bool insert(size_t row, size_t col, const std::string& s);
    bool erase(size_t row, size_t col, size_t len);
    bool new_line(size_t row);
    bool delete_line(size_t row);
public:
    Buffer();
    bool transform(Transformation* t);
    bool undo();
    bool redo();
    void draw(size_t first, size_t n);
    const std::string& get_line(size_t row) const;
    size_t get_num_lines() const;
};
