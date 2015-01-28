#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "graphics.hpp"
#include "buffer.hpp"
#include "config.hpp"

Line::Line():
    texture {nullptr},
    rect {0,0,0,0},
    prev {nullptr},
    content {},
    next {nullptr},
    modified {true}
{
}

void Line::update_texture()
{
    SDL_Surface* s = TTF_RenderText_Blended(Graphics::font, content.c_str(), Config::font_color);
    if (!s) { /* TODO do some error reporting here */ return; }

    rect.w = s->w;
    rect.h = s->h;
    SDL_Texture* t = SDL_CreateTextureFromSurface(Graphics::renderer, s);
    
    /* we've created our texture from the surface, and no longer need the surface */
    SDL_FreeSurface(s);

    if (!t) { /* TODO do some error reporting here, too */ return; }
    
    /* if we've gotten here, creating the new texture worked
     * so we just delete the old one and replace it with the new
     */
    modified = false;
    if (texture) SDL_DestroyTexture(texture);
    texture = t;
}

SDL_Texture* Line::get_texture()
{
    if (modified) update_texture();
    return texture;
}

void Line::modify()
{
    modified = true;
}

Buffer::Buffer():
    current {new Line},
    current_line {1},
    num_lines    {1},
    history {},
    history_index {-1}
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

NewLine::NewLine(size_t row):
    row {row}
{
}

DeleteLine::DeleteLine(size_t row):
    row {row}
{
}

std::unique_ptr<Transformation> Insert::inverted() const
{
    return std::make_unique<Delete>(s, row, col);
}

std::unique_ptr<Transformation> Delete::inverted() const
{
    return std::make_unique<Insert>(s, row, col);
}

std::unique_ptr<Transformation> NewLine::inverted() const
{
    return std::make_unique<DeleteLine>(row);
}

std::unique_ptr<Transformation> DeleteLine::inverted() const
{
    return std::make_unique<NewLine>(row);
}

bool Insert::apply(Buffer& b) const
{
    return b.insert(row, col, s);
}

bool Delete::apply(Buffer& b) const
{
    return b.erase(row, col, s.length());
}

bool NewLine::apply(Buffer& b) const
{
    return b.new_line(row);
}

bool DeleteLine::apply(Buffer& b) const
{
    return b.delete_line(row);
}

bool Buffer::go_to_line(size_t row) const
{
    if (row > num_lines || row < 1) return false;

    while (row < current_line) {
	current = current->prev;
	--current_line;
	if (!current) return false;
    }
    while (row > current_line) {
	current = current->next;
	++current_line;
	if (!current) return false;
    }

    return true;
}

bool Buffer::insert(size_t row, size_t col, const std::string& s)
{
    if (go_to_line(row)) {
	if (current->content.length() < col - 1) return false;
	current->content.insert(col - 1, s);
	current->modify();
	return true;
    } else return false;
}

bool Buffer::erase(size_t row, size_t col, size_t len)
{
    if (!go_to_line(row)) return false;

    if (current->content.length() + 1 < col + len) return false;
    
    current->content.erase(col - 1, len);
    current->modify();
    return true;
}

bool Buffer::new_line(size_t row)
{
    /* special case of when row == 1
     * we must update the first_line entry
     */
    if (row == 1) {
	Line* l = new Line;
	l->next = first;
	first->prev = l;
	first = l;
	++num_lines;
	current_line += 1;
	return true;
    }

    if (!go_to_line(row - 1)) return false;

    Line* l = new Line;
    l->prev = current;
    l->next = current->next;
    current->next = l;
    ++num_lines;
    current = l;

    if (!current->next)
	last = current;

    ++current_line;

    return true;
}

bool Buffer::transform(Transformation* t)
{
    history.emplace_back(t);
    history_index += 1;
    return t->apply(*this);
}

bool Buffer::undo()
{
    if (history_index == -1) return false; // There is nothing to be undone

    history[history_index]->inverted()->apply(*this);

    history_index -= 1;

    return true;
}

bool Buffer::redo()
{
    if (history_index + 1 == (int)history.size()) return false; // There is nothing to be redone

    history_index += 1;

    history[history_index]->apply(*this);

    return true;
}

bool Buffer::delete_line(size_t row)
{
    if (row > num_lines || row < 1 || num_lines == 1) return false;

    if (!go_to_line(row)) return false;

    if (current->content.length()) return false;

    if (current->next) current->next->prev = current->prev;
    if (current->prev) current->prev->next = current->next;

    auto old_current = current;

    current = row == 1 ? current->next : current->prev;
    current_line = row == 1 ? 1 : row - 1;

    num_lines -= 1;

    delete old_current;

    return true;
}

void Buffer::draw(size_t fst, size_t n)
{
    if (fst < 1 || fst > num_lines) return;

    Line* original = current;
    size_t original_number = current_line;

    if (!go_to_line(fst)) return;

    /* here we draw the line textures */
    SDL_Rect rect {0,0,0,0};
    for (size_t i = 0; i < n && current; ++i, current = current->next) {
	if (current->content.length() == 0) continue;
	rect.w = current->rect.w;
	rect.h = current->rect.h;
	SDL_RenderCopy(Graphics::renderer, current->get_texture(), nullptr, &rect);
	rect.y += rect.h;
    }
    
    current = original;
    current_line = original_number;
}

const std::string& Buffer::get_line(size_t row) const
{
    if (!go_to_line(row)) return nullptr;
    return current->content;
}

size_t Buffer::get_num_lines() const
{
    return num_lines;
}
