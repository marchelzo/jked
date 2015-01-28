#include "graphics.hpp"

SDL_Window* Graphics::window {nullptr};
SDL_Renderer* Graphics::renderer {nullptr};
TTF_Font* Graphics::font {nullptr};

bool Graphics::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1) return false;
    if (TTF_Init()               == -1) return false;
    if (!(Graphics::window = SDL_CreateWindow("jked", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
						800, 600, SDL_WINDOW_SHOWN))) return false;
    if (!(Graphics::renderer = SDL_CreateRenderer(Graphics::window, -1,
						  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) return false;
    SDL_SetRenderDrawColor(Graphics::renderer, 200, 200, 200, 255);
    return true;
}

bool Graphics::set_font(const std::string& font_name)
{
    TTF_Font* f = TTF_OpenFont(font_name.c_str(), 14);
    if (!f) return false;

    TTF_CloseFont(Graphics::font);
    Graphics::font = f;

    return true;
}

void Graphics::quit()
{
    TTF_Quit();
    SDL_Quit();
}
