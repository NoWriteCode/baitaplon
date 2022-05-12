#include "snake.hpp"

#include <string>

#include <SDL2/SDL_image.h>

void Snake::renderImage(std::string path)
{
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_FreeSurface(loadedSurface);
	SDL_DestroyTexture(texture);
}
