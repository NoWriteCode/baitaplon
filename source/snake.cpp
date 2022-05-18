#include "snake.hpp"
#include <SDL2/SDL_ttf.h>

int Snake::snakeBodyLength = 3;

Snake::Snake() {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	window = SDL_CreateWindow("Snake Xenzia", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	score = 0;
	highscore = 0;
}

Snake::~Snake() {
    SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
    SDL_DestroyTexture(this->texture);
    SDL_Quit();
}