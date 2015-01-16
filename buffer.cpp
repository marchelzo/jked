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
    return std::make_unique<DeleteLine>();
}

std::unique_ptr<Transformation> DeleteLine::inverted() const
{
    return std::make_unique<NewLine>();
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

bool Buffer::erase(size_t row, size_t col, const std::string& s)
{

}

bool Buffer::new_line(size_t row)
{

}

bool Buffer::delete_line(size_t row)
{

}

int main()
{

}
