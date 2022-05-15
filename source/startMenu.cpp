#include "snake.hpp"

void Snake::startMenu(int button) {
	
	score = 0;
    char a = button + '0';
	std::string s;
	s.push_back(a);
	renderImage("image/start" + s + ".png");

	enum {
		START,
		HIGHSCORES,
		EXIT
	};

	bool quit = false;
	// Event handler
	SDL_Event e;
	// While application is running
	while (!quit) {
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			// User requests quitb
			if (e.type == SDL_QUIT)
			{
				quit = true;
				exit(0);
				break;
			}

			else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_UP && button > 0) {
					button--;
				}

				if (e.key.keysym.sym == SDLK_DOWN && button < 2) {
					button++;
				}

				if (button == START) {
					renderImage("image/start0.png");
				}

				if (button == HIGHSCORES) {
					renderImage("image/start1.png");
				}

				if (button == EXIT) {
					renderImage("image/start2.png");
				}

				if (e.key.keysym.sym == SDLK_RETURN && button == START) {
					loadSound("sound/StartGameSound.wav");
					playGame();
					quit = true;
				}

				if (e.key.keysym.sym == SDLK_RETURN && button == HIGHSCORES) {
					highScore(e);
				}

				if (e.key.keysym.sym == SDLK_RETURN && button == EXIT) {
					quit = true;
					exit(0);
					break;
				}
			}

			else if (e.type == SDL_MOUSEMOTION) {
				if (e.button.x < 440 && e.button.x > 150 && e.button.y > 250 && e.button.y < 272) {
					renderImage("image/start0.png");
					button = START;
				}

				if (e.button.x < 451 && e.button.x > 137 && e.button.y > 305 && e.button.y < 329) {
					renderImage("image/start1.png");
					button = HIGHSCORES;
				}

				if (e.button.x < 350 && e.button.x > 237 && e.button.y > 361 && e.button.y < 386) {
					renderImage("image/start2.png");
					button = EXIT;
				}
			}

			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					if (button == START) {
						loadSound("sound/StartGameSound.wav");
						playGame();
						quit = true;
					}

					if (button == HIGHSCORES) {
						highScore(e);
					}

					if (button == EXIT) {
						quit = true;
						exit(0);
						break;
					}
				}
			}
		}
	}
}