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

	//background
	SDL_SetRenderDrawColor(renderer, 155, 45, 65, 255);
	SDL_RenderClear(renderer);

	//ve map
	drawMap();
	
	// ve lai cai dau
	SDL_SetRenderDrawColor(renderer, 255, 55, 55, 255);
	SDL_RenderFillRect(renderer, &snakeHead);

	SDL_SetRenderDrawColor(renderer, 162, 229, 123, 0xFF);
	for (int i = 0; i < snakeBodyLength; i++) {
		SDL_RenderFillRect(renderer, &snakeBody[i]);
	}

	int flag = 0;

	for (int i = 0; i < std::max(snakeBodyLength, 260); i++) {
		if ((snakeHead.x == snakeBody[i].x && snakeHead.y == snakeBody[i].y) || (snakeHead.x == wall[i].x && snakeHead.y == wall[i].y)) {
			snakeBodyLength = 3;
			flag = 1;
		}
	}

	//khi an dc point
	if (snakeHead.x == point.x && snakeHead.y == point.y) {
		score = score + 1;

		if (score == 1) loadSound("sound/FirstBlood.mp3");
		else if (score % 5 == 2) loadSound("sound/doubleKill.mp3");
		else if (score % 5 == 3) loadSound("sound/tripleKill.mp3");
		else if (score % 5 == 4) loadSound("sound/quataKill.mp3");
		else if (score % 5 == 0) loadSound("sound/pentaKill.mp3");
		else loadSound("sound/kill.mp3");

		// chuyển point ra chỗ khác
		point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
		point.y = (rand() % 48) * 10;
		
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
		
		snakeBodyLength++;
		snakeBody[snakeBodyLength - 1].x = snakeBody[snakeBodyLength - 2].x;
		snakeBody[snakeBodyLength - 1].y = snakeBody[snakeBodyLength - 2].y;
	}

	// vẽ lại cái để ăn
	SDL_SetRenderDrawColor(renderer, 20, 45, 80, 0xFF);
	SDL_RenderFillRect(renderer, &point);

	std::string s;
	std::stringstream ss;
	ss << score;
	ss >> s;

	loadText("font/PressStart2P.ttf", 17, "         SCORE:" + s, {255, 255, 255, 255}, 50, 510, 200, 30);
	SDL_RenderPresent(renderer);
	SDL_Delay(50);
	return flag;
}