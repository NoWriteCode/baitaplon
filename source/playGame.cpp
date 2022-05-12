#include "snake.hpp"
#include <ctime>
#include <sstream>
#include <fstream>

void Snake::playGame() {

	// tạo phần đầu
	SDL_Rect snakeHead;
	snakeHead.x = SCREEN_HEIGHT / 2;
	snakeHead.y = SCREEN_WIDTH / 2;
	snakeHead.w = 10;
	snakeHead.h = 10;
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderFillRect(renderer, &snakeHead);

	// tạo phần thân
	const int snakeMaxLength = SCREEN_HEIGHT * SCREEN_WIDTH / 100;
	SDL_Rect snakeBody[snakeMaxLength];
	
	// tạo tường
	SDL_Rect wall[220];
	for (int i = 0; i < snakeMaxLength; i++)
	{
		snakeBody[i].w = 10;
		snakeBody[i].h = 10;
	}
	for (int i = 0; i < 220; i++)
	{
		wall[i].w = 10;
		wall[i].h = 10;
	}

	snakeBody[0].x = snakeHead.x - 10;
	snakeBody[0].y = snakeHead.y;
	snakeBody[1].x = snakeHead.x - 20;
	snakeBody[1].y = snakeHead.y;
	snakeBody[2].x = snakeHead.x - 30;
	snakeBody[2].y = snakeHead.y;
	SDL_SetRenderDrawColor(renderer, 102, 255, 51, 0xFF);
	for (int i = 0; i < 3; i++) {
		SDL_RenderFillRect(renderer, &snakeBody[i]);
	}

	// SDL_RenderFillRect(renderer, &snakeBody[0]);
	// SDL_RenderFillRect(renderer, &snakeBody[1]);
	// SDL_RenderFillRect(renderer, &snakeBody[2]);

	// ve map
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

	SDL_RenderPresent(renderer);

	// tạo điểm ngẫu nhiên để ăn
	SDL_Rect point;
	point.w = 10;
	point.h = 10;
	srand(time(NULL));
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

	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 0xFF);
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

		// nếu đâm vào đuôi
		for (int i = 0; i < std::max(snakeBodyLength, 220); i++) {
			if ((snakeHead.x == snakeBody[i].x && snakeHead.y == snakeBody[i].y) || (snakeHead.x == wall[i].x && snakeHead.y == wall[i].y)) {
				snakeBodyLength = 3;
				SDL_Event event;
				bool quit2 = false;
				while (!quit2) {
					renderImage("image/GameOver.png");
					if (SDL_WaitEvent(&event)) {
						if (event.type == SDL_QUIT) {
							quit2 = true;
							quit = true;
							exit(0);
							break;
						}

						if (event.key.keysym.sym == SDLK_y) {
							playGame();
						}

						if (event.key.keysym.sym == SDLK_n) {
							quit2 = true;
							quit = true;
							exit(0);
							break;
						}
					}
				}
			}
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

								if (e.key.keysym.sym == SDLK_UP && button == 1) {
									button = 0;
									renderImage("image/pause0.png");
									loadText("font/PressStart2P.ttf", 26, s, {255, 255, 255, 255}, 305, 90, 200, 40);
								}

								if (e.key.keysym.sym == SDLK_DOWN && button == 0) {
									button = 1;
									renderImage("image/pause1.png");
									loadText("font/PressStart2P.ttf", 26, s, {255, 255, 255, 255}, 305, 90, 200, 40);
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

				// dua phan sau ve trc
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
				// neu an duoc diem
				if (snakeHead.x == point.x && snakeHead.y == point.y) {
					loadSound("sound/Move.wav");
					score = score + 10;
					// chuyển point ra chỗ khác
					point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
					point.y = (rand() % 48) * 10;

					// xu li diem o tren tuong
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
				if (snakeMoves(snakeHead, snakeBody, wall, point, snakeBodyLength, direction)) {
					gameOver();
					quit = true;
				}
			}
		}
	}
}