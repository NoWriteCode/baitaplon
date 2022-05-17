#include "snake.hpp"
#include <SDL2/SDL_ttf.h>
#include <ctime>

Snake::Snake() {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	window = SDL_CreateWindow("Snake Xenzia", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	score = 0;
	highscore = 0;
}

int Snake::snakeBodyLength = 3;

void Snake::drawPoint() {
    SDL_Rect point;
	point.w = 10;
	point.h = 10;
	srand(time(NULL));
	point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
	point.y = (rand() % 48) * 10;

	//xy li diem tren tuong
	bool pointOnWall = false;
	for (int j = 0; j < 260; j++) {
		if (point.x == wall[j].x && point.y == wall[j].y) {
			pointOnWall = true;
			break;
		}
	}

    while (pointOnWall) {
		bool check3 = true;
		point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
		point.y = (rand() % 48) * 10;
		for (int j = 0; j < 260; j++) {
			if (point.x == wall[j].x && point.y == wall[j].y) {
				check3 = false;
				break;
			}
		}

		if (check3) {
			pointOnWall = false;
		}
	}
	SDL_SetRenderDrawColor(renderer, 20, 45, 80, 0xFF);
	SDL_RenderFillRect(renderer, &point);
	SDL_RenderPresent(renderer);
}

Snake::~Snake() {
    SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
    SDL_DestroyTexture(this->texture);
    SDL_Quit();
}