#include <ncurses.h>
#include <string>

#include "graphics.hpp"
#include "buffer.hpp"

int main(int argc, char *argv[])
{
    Buffer b;

    if (!Graphics::init()) return 1;
    if (!Graphics::set_font("/Library/Fonts/Courier New.ttf")) return 1;

    size_t line{1};

    int c;

    while ((c = getchar()) != 'q') {

	SDL_RenderClear(Graphics::renderer);

	if (c == '\n' || c == '\r') {
	    b.transform(new NewLine{++line});
	} else if (c == 'i') {
	    b.transform(new Insert{"Hello", line, 1});
	}

	b.draw(1,10);

	SDL_RenderPresent(Graphics::renderer);
    }

    Graphics::quit();
}
