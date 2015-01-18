#include <iostream>

#include "buffer.hpp"

Line::Line():
    prev {nullptr},
    content {},
    next {nullptr}
{
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

bool Buffer::go_to_line(size_t row)
{
    if (row > num_lines || row < 1) return false;

    if (row > current_line) {
	while (row > current_line) {
	    current = current->prev;
	    --current_line;
	    if (!current) return false;
	}
    } else {
	while (row < current_line) {
	    current = current->next;
	    ++current_line;
	    if (!current) return false;
	}
    }
    return true;
}

bool Buffer::insert(size_t row, size_t col, const std::string& s)
{
    if (go_to_line(row)) {
	if (current->content.length() < col - 1) return false;
	current->content.insert(col - 1, s);
	return true;
    } else return false;
}

bool Buffer::erase(size_t row, size_t col, size_t len)
{
    if (!go_to_line(row)) return false;

    if (current->content.length() + 1 < col + len) return false;
    
    current->content.erase(col - 1, len);
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

void Buffer::draw()
{
    while (current->prev)
	current = current->prev;

    while (current->next) {
	std::cout << current->content << '\n';
	current = current->next;
    }

    std::cout << current->content << '\n';

    current_line = num_lines;
}
