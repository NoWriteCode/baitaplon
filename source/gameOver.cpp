#include "snake.hpp"
#include <fstream>
#include <sstream>

void Snake::gameOver() {
	//
    loadSound("sound/defeat.wav");
	if (score > highscore) {
		std::ofstream fout;
		fout.open("highscore.txt");
		fout << score;
		fout.close(); 
	}

	SDL_Event event;
	bool quit2 = false;

	while (!quit2) {
		std::string s;
		std::stringstream ss;
		ss << score;
		ss >> s;
		renderImage("image/gameover0.png");
		loadText("font/PressStart2P.ttf", 35, "SCORE:" + s, {255, 255, 255, 255}, 180, 90, 200, 40);

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

				else if (e.type == SDL_KEYDOWN) {

					if (e.key.keysym.sym == SDLK_ESCAPE) {
						quit3 = true;
						break;
					}

					if (e.key.keysym.sym == SDLK_UP && button == 1) {
						button = 0;
						renderImage("image/gameover0.png");
						loadText("font/PressStart2P.ttf", 35, "SCORE:" + s, {255, 255, 255, 255}, 180, 90, 200, 40);
					}

					if (e.key.keysym.sym == SDLK_DOWN && button == 0) {
						button = 1;
						renderImage("image/gameover1.png");
						loadText("font/PressStart2P.ttf", 35, "SCORE:" + s, {255, 255, 255, 255}, 180, 90, 200, 40);
					}

					if (e.key.keysym.sym == SDLK_RETURN && button == 1) {
						snakeBodyLength = 3;
						startMenu(0);
					}

					if (e.key.keysym.sym == SDLK_RETURN && button == 0) {
						loadSound("sound/LOL.wav");
						score = 0;
						playGame();
					}
				}
			}
		}
	}
}