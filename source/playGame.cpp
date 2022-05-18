#include "snake.hpp"

#include <ctime>
#include <sstream>
#include <fstream>

void Snake::playGame() {
	// tạo phần đầu
	SDL_Rect snakeHead;
	snakeHead.x = SCREEN_HEIGHT / 2;
	snakeHead.y = SCREEN_WIDTH / 2 - 60;
	snakeHead.w = 10;
	snakeHead.h = 10;
	SDL_SetRenderDrawColor(renderer, 255, 55, 55, 255);
	SDL_RenderFillRect(renderer, &snakeHead);

	// tạo phần thân
	const int snakeMaxLength = SCREEN_HEIGHT * SCREEN_WIDTH / 100;
	SDL_Rect snakeBody[snakeMaxLength];
	
	for (int i = 0; i < snakeMaxLength; i++) {
		snakeBody[i].w = 10;
		snakeBody[i].h = 10;
	}

	for (int i = 0; i < snakeBodyLength; i++) {
		snakeBody[i].x = snakeHead.x - 10 * i;
		snakeBody[i].y = snakeHead.y;
	}
	SDL_SetRenderDrawColor(renderer, 162, 229, 123, 0xFF);
	for (int i = 0; i < 3; i++) {
		SDL_RenderFillRect(renderer, &snakeBody[i]);
	}
	SDL_RenderPresent(renderer);

	// tạo điểm ngẫu nhiên để ăn
	SDL_Rect point;
	point.w = 10;
	point.h = 10;
	srand(time(NULL));
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

	SDL_SetRenderDrawColor(renderer, 162, 229, 123, 0xFF);
	SDL_RenderFillRect(renderer, &point);
	SDL_RenderPresent(renderer);

	int direction = RIGHT;
	std::string s;
	std::stringstream ss;
	ss << score;
	ss >> s;

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		if (e.type == SDL_QUIT) {
			quit = true;
			break;
		}

		if (snakeMoves(snakeHead, snakeBody, wall, point, snakeBodyLength, direction)) {
			gameOver();
			quit = true;
		}
		
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			// User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
				exit(0);
				break;
			}

			else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					std::string s;
					std::stringstream ss;
					ss << score;
					ss >> s;
					renderImage("image/pause0.png");
					loadText("font/PressStart2P.ttf", 26, s, {255, 255, 255, 255}, 305, 90, 200, 40);
					bool quit3 = false;
					// Event handler
					SDL_Event e;
					int button = 0;
					// While application is running
					while (!quit3) {
						// Handle events on queue
						while (SDL_PollEvent(&e) != 0) {
							// User requests quit
							if (e.type == SDL_QUIT) {
								quit3 = true;
								exit(0);
								break;
							}

							// User presses a key
							else if (e.type == SDL_KEYDOWN) {

								if (e.key.keysym.sym == SDLK_ESCAPE) {
									quit3 = true;
									break;
								}

								if (e.key.keysym.sym == SDLK_RETURN && button == 1) {
									snakeBodyLength = 3;
									startMenu(0);
								}

								if (e.key.keysym.sym == SDLK_RETURN && button == 0) {
									quit3 = true;
									break;
								}
							}
						}
					}
				}

				// move
				for (int i = snakeBodyLength - 1; i > 0; i--) {
					snakeBody[i].x = snakeBody[i - 1].x;
					snakeBody[i].y = snakeBody[i - 1].y;
				}

				snakeBody[0].x = snakeHead.x;
				snakeBody[0].y = snakeHead.y;

				if (direction == LEFT) {
					if (e.key.keysym.sym == SDLK_DOWN) {
						direction = DOWN;
					}

					if (e.key.keysym.sym == SDLK_UP) {
						direction = UP;
					}
				}
				
				if (direction == RIGHT) {

					if (e.key.keysym.sym == SDLK_DOWN) {
						direction = DOWN;
					}

					if (e.key.keysym.sym == SDLK_UP) {
						direction = UP;
					}
				}

				if (direction == UP) {
					if (e.key.keysym.sym == SDLK_LEFT) {
						direction = LEFT;
					}
					// Tương tự với dịch phải, xuống và lên
					if (e.key.keysym.sym == SDLK_RIGHT) {
						direction = RIGHT;
					}
				}

				if (direction == DOWN) {
					if (e.key.keysym.sym == SDLK_LEFT) {
						direction = LEFT;
					}

					if (e.key.keysym.sym == SDLK_RIGHT) {
						direction = RIGHT;
					}
				}

				if (snakeMoves(snakeHead, snakeBody, wall, point, snakeBodyLength, direction)) {
					gameOver();
					quit = true;
				}
			}
		}
	}
}