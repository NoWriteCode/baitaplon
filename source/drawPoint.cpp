#include "snake.hpp"

#include <ctime>

void Snake::drawPoint(SDL_Rect &point) {
    //
	point.w = 10;
	point.h = 10;
	srand(time(NULL));
	point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
	point.y = (rand() % 48) * 10;

	//xy li diem tren tuong
	bool pointOnWall = false;
	for (int j = 0; j < 261; j++) {
		if (point.x == wall[j].x && point.y == wall[j].y) {
			pointOnWall = true;
			break;
		}
	}

    while (pointOnWall) {
		bool check3 = true;
		point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
		point.y = (rand() % 48) * 10;
		for (int j = 0; j < 261; j++) {
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