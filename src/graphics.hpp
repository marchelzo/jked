#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

/* This is a collection of global state pertaining to SDL
 * and resources involved with its usage
 */

namespace Graphics {
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern TTF_Font* font;
    bool set_font(const std::string& font_name);
    bool init();
    void quit();
}
