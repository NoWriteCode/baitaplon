#include "snake.hpp"

void Snake::loadText(const std::string &font_path, int font_size, const std::string &text, const SDL_Color color, int x, int y, int w, int h) {
	TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);
	auto textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	auto textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	TTF_SizeText(font, text.c_str(), &w, &h);
	SDL_Rect rect = {x, y, w, h};

	SDL_RenderCopy(renderer, textTexture, NULL, &rect);
	SDL_FreeSurface(textSurface);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(textTexture);
}
