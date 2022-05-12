#include "snake.hpp"

#include <fstream>
#include <sstream>

void Snake::hscore(SDL_Event e) {
	
	renderImage("highscore1.png");
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	std::ifstream myfile("source/highscore.txt");
	std::string line;
	if (myfile.is_open())
	{
		myfile >> highscore;
	}
	myfile.close();
	std::string s;
	std::stringstream ss;
	ss << highscore;
	ss >> s;
	
	loadText("font/PressStart2P.ttf", 22, "HIGH SCORE: " + s, {255, 255, 255, 255}, 60, 200, 30, 30);
	loadText("font/PressStart2P.ttf", 22, "(Press any key to exit) ", {255, 255, 255, 255}, 60, 250, 30, 30);
	loadText("font/PressStart2P.ttf", 22, "       < BACK ", {255, 255, 255, 255}, 60, 500, 30, 30);

	int flag = 0;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_QUIT)	{
				quit = true;
				exit(0);
				break;
			}

			else if (e.type == SDL_MOUSEBUTTONDOWN)	{
				if ((e.button.button == SDL_BUTTON_LEFT) && (e.button.x < 346 && e.button.x > 215 && e.button.y > 500 && e.button.y < 520))
				{
					startMenu(1);
					quit = true;
					break;
				}
			}

			else if (e.type == SDL_KEYDOWN) {
				startMenu(1);
				quit = true;
				break;
			}
		}
	}
}