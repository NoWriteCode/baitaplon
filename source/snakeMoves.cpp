#include "snake.hpp"
#include <sstream>
#include <fstream>

int Snake::snakeMoves(SDL_Rect &snakeHead, SDL_Rect *snakeBody, SDL_Rect *wall, SDL_Rect &point, int &snakeBodyLength, int direction) {

	// di chuyển phần thân
	for (int i = snakeBodyLength - 1; i > 0; i--) {
		snakeBody[i].x = snakeBody[i - 1].x;
		snakeBody[i].y = snakeBody[i - 1].y;
	}

	snakeBody[0].x = snakeHead.x;
	snakeBody[0].y = snakeHead.y;

	if (direction == LEFT) {
		snakeHead.x = (snakeHead.x + SCREEN_WIDTH - step) % SCREEN_WIDTH;
	}

	else if (direction == RIGHT) {
		snakeHead.x = (snakeHead.x + step) % SCREEN_WIDTH;
	}

	else if (direction == DOWN) {
		snakeHead.y = (snakeHead.y + step) % SCREEN_HEIGHT;
	}

	else if (direction == UP) {
		snakeHead.y = (snakeHead.y + SCREEN_HEIGHT - step) % SCREEN_HEIGHT;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (int i = 0; i < 220; i++) {
		wall[i].w = 10;
		wall[i].h = 10;
	}

	for (int i = 0; i < 60; i++) {
		wall[i].y = 0;
		wall[i].x = i * 10;
	}

	for (int i = 60; i < 120; i++) {
		wall[i].y = 480;
		wall[i].x = (i - 60) * 10;
	}

	for (int i = 1; i <= 15; i++) {
		wall[i + 119].x = 0;
		wall[i + 119].y = i * 10;
	}

	for (int i = 34; i <= 48; i++) {
		wall[i + 101].x = 0;
		wall[i + 101].y = i * 10;
	}

	for (int i = 1; i <= 15; i++) {
		wall[i + 150].x = 590;
		wall[i + 150].y = i * 10;
	}

	for (int i = 34; i <= 48; i++) {
		wall[i + 132].x = 590;
		wall[i + 132].y = i * 10;
	}

	for (int i = 20; i < 39; i++) {
		wall[i + 161].x = i * 10;
		wall[i + 161].y = 200;
	}

	for (int i = 20; i < 39; i++) {
		wall[i + 180].x = i * 10;
		wall[i + 180].y = 280;
	}
	SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
	for (int i = 0; i < 220; i++) {
		SDL_RenderFillRect(renderer, &wall[i]);
	}

	// ve lai cai dau
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderFillRect(renderer, &snakeHead);
	SDL_SetRenderDrawColor(renderer, 102, 255, 51, 0xFF);
	for (int i = 0; i < snakeBodyLength; i++) {
		SDL_RenderFillRect(renderer, &snakeBody[i]);
	}

	int flag = 0;
	for (int i = 0; i < std::max(snakeBodyLength, 220); i++) {
		if ((snakeHead.x == snakeBody[i].x && snakeHead.y == snakeBody[i].y) || (snakeHead.x == wall[i].x && snakeHead.y == wall[i].y)) {
			snakeBodyLength = 3;
			flag = 1;
		}
	}

	if (snakeHead.x == point.x && snakeHead.y == point.y) {
		score = score + 1;
		loadSound("sound/Move.wav");
		// chuyển point ra chỗ khác
		point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
		point.y = (rand() % 48) * 10;
		bool pointOnWall = false;
		for (int j = 0; j < 220; j++) {
			if (point.x == wall[j].x && point.y == wall[j].y) {
				pointOnWall = true;
				break;
			}
		}
		while (pointOnWall) {
			bool check3 = true;
			point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
			point.y = (rand() % 48) * 10;
			for (int j = 0; j < 220; j++) {
				if (point.x == wall[j].x && point.y == wall[j].y) {
					check3 = false;
					break;
				}
			}
			if (check3) {
				pointOnWall = false;
			}
		}
		
		snakeBodyLength++;
		snakeBody[snakeBodyLength - 1].x = snakeBody[snakeBodyLength - 2].x;
		snakeBody[snakeBodyLength - 1].y = snakeBody[snakeBodyLength - 2].y;
	}

	// vẽ lại cái để ăn
	SDL_SetRenderDrawColor(renderer, 255, 51, 0, 0xFF);
	SDL_RenderFillRect(renderer, &point);
	std::string s;
	std::stringstream ss;
	ss << score;
	ss >> s;

	loadText("font/PressStart2P.ttf", 17, "SCORE:" + s, {100, 100, 100, 255}, 50, 510, 200, 30);
	SDL_RenderPresent(renderer);
	SDL_Delay(50);
	return flag;
}