#include "buffer.hpp"

int main(int argc, char *argv[])
{
    Buffer b;

    b.transform(new Insert{"Hello, world!", 1, 1});
    b.transform(new Insert{"free ", 1, 8});
    b.transform(new NewLine{2});
    b.undo();
    b.redo();
    
    b.draw();
}
