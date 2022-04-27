#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
using namespace std;

// Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Current displayed texture
SDL_Texture *gTexture = NULL;
SDL_Surface *gScreenSurface = NULL;

int step = 10;
int snakeBodyLength = 3;
const int total_wall = 220;
int score = 0;
int highscore;

enum direction
{
	RIGHT,
	LEFT,
	UP,
	DOWN
};

// void snakeMove(SDL_Rect &snakeHead, SDL_Rect *snakeBody, SDL_Rect &point, int &snakeBodyLength, int direction);

// di chuyen con ran theo 1 huong xac dinh
int snakeMoves(SDL_Rect &snakeHead, SDL_Rect *snakeBody, SDL_Rect *wall, SDL_Rect &point, int &snakeBodyLength, int direction);

void renderImage(string path);

void loadSound(string path);

void playGame();

void loadText(const string &font_path, int font_size, const string &text, const SDL_Color color, int x, int y, int w, int h)
{
	TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);
	auto textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	auto textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	TTF_SizeText(font, text.c_str(), &w, &h);
	SDL_Rect rect = {x, y, w, h};
	SDL_RenderCopy(gRenderer, textTexture, NULL, &rect);
	SDL_FreeSurface(textSurface);
	SDL_RenderPresent(gRenderer);
	SDL_DestroyTexture(textTexture);
}

void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = nullptr;
	// Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	// Quit SDL subsystems
	SDL_Quit();
}

void hscore(SDL_Event e);

void gameover();

void startmenu(int button)
{
	char a = button + '0';
	string s;
	s.push_back(a);
	renderImage("image/start" + s + ".png");
	enum
	{
		START,
		HIGHSCORES,
		EXIT
	};
	bool quit = false;

	// Event handler
	SDL_Event e;
	// While application is running
	while (!quit)
	{
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{

			// User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
				exit(0);
				break;
			}

			// User presses a key
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_UP && button > 0)
				{
					button--;
				}
				if (e.key.keysym.sym == SDLK_DOWN && button < 2)
				{
					button++;
				}
				if (button == START)
				{
					renderImage("image/start0.png");
				}
				if (button == HIGHSCORES)
				{
					renderImage("image/start1.png");
				}
				if (button == EXIT)
				{
					renderImage("image/start2.png");
				}
				if (e.key.keysym.sym == SDLK_RETURN && button == START)
				{
					loadSound("sound/StartGameSound.wav");
					playGame();
					quit = true;
				}
				if (e.key.keysym.sym == SDLK_RETURN && button == HIGHSCORES)
				{
					hscore(e);
				}
				if (e.key.keysym.sym == SDLK_RETURN && button == EXIT)
				{
					quit = true;
					exit(0);

					break;
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				if (e.button.x < 440 && e.button.x > 150 && e.button.y > 250 && e.button.y < 272)
				{
					renderImage("image/start0.png");
					button = START;
				}
				if (e.button.x < 451 && e.button.x > 137 && e.button.y > 305 && e.button.y < 329)
				{
					renderImage("image/start1.png");
					button = HIGHSCORES;
				}
				if (e.button.x < 350 && e.button.x > 237 && e.button.y > 361 && e.button.y < 386)
				{
					renderImage("image/start2.png");
					button = EXIT;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (button == START)
					{
						loadSound("sound/StartGameSound.wav");
						playGame();
						quit = true;
					}
					if (button == HIGHSCORES)
					{
						hscore(e);
					}
					if (button == EXIT)
					{
						quit = true;
						exit(0);
						break;
					}
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	gWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	TTF_Init();

	startmenu(0);
	close();
	return 0;
}

int snakeMoves(SDL_Rect &snakeHead, SDL_Rect *snakeBody, SDL_Rect *wall, SDL_Rect &point, int &snakeBodyLength, int direction)
{
	// di chuyển phần thân
	for (int i = snakeBodyLength - 1; i > 0; i--)
	{
		snakeBody[i].x = snakeBody[i - 1].x;
		snakeBody[i].y = snakeBody[i - 1].y;
	}
	snakeBody[0].x = snakeHead.x;
	snakeBody[0].y = snakeHead.y;

	if (direction == LEFT)
	{
		snakeHead.x = (snakeHead.x + SCREEN_WIDTH - step) % SCREEN_WIDTH;
	}
	else if (direction == RIGHT)
	{
		snakeHead.x = (snakeHead.x + step) % SCREEN_WIDTH;
	}
	else if (direction == DOWN)
	{
		snakeHead.y = (snakeHead.y + step) % SCREEN_HEIGHT;
	}
	else if (direction == UP)
	{
		snakeHead.y = (snakeHead.y + SCREEN_HEIGHT - step) % SCREEN_HEIGHT;
	}

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);

	for (int i = 0; i < total_wall; i++)
	{
		wall[i].w = 10;
		wall[i].h = 10;
	}
	for (int i = 0; i < 60; i++)
	{
		wall[i].y = 0;
		wall[i].x = i * 10;
	}
	for (int i = 60; i < 120; i++)
	{
		wall[i].y = 480;
		wall[i].x = (i - 60) * 10;
	}
	for (int i = 1; i <= 15; i++)
	{
		wall[i + 119].x = 0;
		wall[i + 119].y = i * 10;
	}
	for (int i = 34; i <= 48; i++)
	{
		wall[i + 101].x = 0;
		wall[i + 101].y = i * 10;
	}
	for (int i = 1; i <= 15; i++)
	{
		wall[i + 150].x = 590;
		wall[i + 150].y = i * 10;
	}
	for (int i = 34; i <= 48; i++)
	{
		wall[i + 132].x = 590;
		wall[i + 132].y = i * 10;
	}
	for (int i = 20; i < 39; i++)
	{
		wall[i + 161].x = i * 10;
		wall[i + 161].y = 200;
	}
	for (int i = 20; i < 39; i++)
	{
		wall[i + 180].x = i * 10;
		wall[i + 180].y = 280;
	}
	SDL_SetRenderDrawColor(gRenderer, 169, 169, 169, 255);
	for (int i = 0; i < total_wall; i++)
	{
		SDL_RenderFillRect(gRenderer, &wall[i]);
	}

	// ve lai cai dau
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
	SDL_RenderFillRect(gRenderer, &snakeHead);
	SDL_SetRenderDrawColor(gRenderer, 102, 255, 51, 0xFF);
	for (int i = 0; i < snakeBodyLength; i++)
	{
		SDL_RenderFillRect(gRenderer, &snakeBody[i]);
	}

	int flag = 0;
	for (int i = 0; i < max(snakeBodyLength, total_wall); i++)
	{
		if ((snakeHead.x == snakeBody[i].x && snakeHead.y == snakeBody[i].y) || (snakeHead.x == wall[i].x && snakeHead.y == wall[i].y))
		{
			snakeBodyLength = 3;
			flag = 1;
		}
	}

	if (snakeHead.x == point.x && snakeHead.y == point.y)
	{
		score = score + 10;
		loadSound("sound/Move.wav");
		// chuyển point ra chỗ khác
		point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
		point.y = (rand() % 48) * 10;
		bool pointOnWall = false;
		for (int j = 0; j < total_wall; j++)
		{
			if (point.x == wall[j].x && point.y == wall[j].y)
			{
				pointOnWall = true;
				break;
			}
		}
		while (pointOnWall)
		{
			bool check3 = true;
			point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
			point.y = (rand() % 48) * 10;
			for (int j = 0; j < total_wall; j++)
			{
				if (point.x == wall[j].x && point.y == wall[j].y)
				{
					check3 = false;
					break;
				}
			}
			if (check3)
			{
				pointOnWall = false;
			}
		}
		snakeBodyLength++;
		snakeBody[snakeBodyLength - 1].x = snakeBody[snakeBodyLength - 2].x;
		snakeBody[snakeBodyLength - 1].y = snakeBody[snakeBodyLength - 2].y;
	}
	// vẽ lại cái để ăn
	SDL_SetRenderDrawColor(gRenderer, 255, 51, 0, 0xFF);
	SDL_RenderFillRect(gRenderer, &point);
	string s;
	stringstream ss;
	ss << score;
	ss >> s;
	loadText("font/PressStart2P.ttf", 17, "SCORE:" + s, {100, 100, 100, 255}, 50, 510, 200, 30);
	SDL_RenderPresent(gRenderer);
	SDL_Delay(50);
	return flag;
}

void renderImage(string path)
{
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_RenderCopy(gRenderer, texture, NULL, NULL);
	SDL_RenderPresent(gRenderer);
	SDL_FreeSurface(loadedSurface);
	SDL_DestroyTexture(texture);
}

void playGame()
{
	score = 0;
	// tạo phần đầu
	SDL_Rect snakeHead;
	snakeHead.x = SCREEN_HEIGHT / 2;
	snakeHead.y = SCREEN_WIDTH / 2;
	snakeHead.w = 10;
	snakeHead.h = 10;
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
	SDL_RenderFillRect(gRenderer, &snakeHead);

	// tạo phần thân
	const int snakeMaxLength = SCREEN_HEIGHT * SCREEN_WIDTH / 100;
	SDL_Rect snakeBody[snakeMaxLength];
	// tạo tường
	SDL_Rect wall[total_wall];
	for (int i = 0; i < snakeMaxLength; i++)
	{
		snakeBody[i].w = 10;
		snakeBody[i].h = 10;
	}
	for (int i = 0; i < total_wall; i++)
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
	SDL_SetRenderDrawColor(gRenderer, 102, 255, 51, 0xFF);
	SDL_RenderFillRect(gRenderer, &snakeBody[0]);
	SDL_RenderFillRect(gRenderer, &snakeBody[1]);
	SDL_RenderFillRect(gRenderer, &snakeBody[2]);

	// tao map
	for (int i = 0; i < total_wall; i++)
	{
		wall[i].w = 10;
		wall[i].h = 10;
	}
	for (int i = 0; i < 60; i++)
	{
		wall[i].y = 0;
		wall[i].x = i * 10;
	}
	for (int i = 60; i < 120; i++)
	{
		wall[i].y = 480;
		wall[i].x = (i - 60) * 10;
	}
	for (int i = 1; i <= 15; i++)
	{
		wall[i + 119].x = 0;
		wall[i + 119].y = i * 10;
	}
	for (int i = 34; i <= 48; i++)
	{
		wall[i + 101].x = 0;
		wall[i + 101].y = i * 10;
	}
	for (int i = 1; i <= 15; i++)
	{
		wall[i + 150].x = 590;
		wall[i + 150].y = i * 10;
	}
	for (int i = 34; i <= 48; i++)
	{
		wall[i + 132].x = 590;
		wall[i + 132].y = i * 10;
	}
	for (int i = 20; i < 39; i++)
	{
		wall[i + 161].x = i * 10;
		wall[i + 161].y = 200;
	}
	for (int i = 20; i < 39; i++)
	{
		wall[i + 180].x = i * 10;
		wall[i + 180].y = 280;
	}
	SDL_SetRenderDrawColor(gRenderer, 169, 169, 169, 255);
	for (int i = 0; i < total_wall; i++)
	{
		SDL_RenderFillRect(gRenderer, &wall[i]);
	}

	SDL_RenderPresent(gRenderer);

	// tạo điểm ngẫu nhiên để ăn
	SDL_Rect point;
	point.w = 10;
	point.h = 10;
	srand(time(NULL));
	point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
	point.y = (rand() % 48) * 10;
	bool pointOnWall = false;
	for (int j = 0; j < total_wall; j++)
	{
		if (point.x == wall[j].x && point.y == wall[j].y)
		{
			pointOnWall = true;
			break;
		}
	}
	while (pointOnWall)
	{
		bool check3 = true;
		point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
		point.y = (rand() % 48) * 10;
		for (int j = 0; j < total_wall; j++)
		{
			if (point.x == wall[j].x && point.y == wall[j].y)
			{
				check3 = false;
				break;
			}
		}
		if (check3)
		{
			pointOnWall = false;
		}
	}
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 0xFF);
	SDL_RenderFillRect(gRenderer, &point);

	SDL_RenderPresent(gRenderer);

	int direction = RIGHT;
	string s;
	stringstream ss;
	ss << score;
	ss >> s;
	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		if (e.type == SDL_QUIT)
		{
			quit = true;
			break;
		}
		if (snakeMoves(snakeHead, snakeBody, wall, point, snakeBodyLength, direction))
		{
			gameover();
			quit = true;
		}

		// nếu đâm vào đuôi
		for (int i = 0; i < max(snakeBodyLength, total_wall); i++)
		{
			if ((snakeHead.x == snakeBody[i].x && snakeHead.y == snakeBody[i].y) || (snakeHead.x == wall[i].x && snakeHead.y == wall[i].y))
			{
				snakeBodyLength = 3;
				SDL_Event event;
				bool quit2 = false;
				while (!quit2)
				{
					renderImage("image/GameOver.png");
					if (SDL_WaitEvent(&event))
					{
						if (event.type == SDL_QUIT)
						{
							quit2 = true;
							quit = true;
							exit(0);
							break;
						}
						if (event.key.keysym.sym == SDLK_y)
						{
							playGame();
						}
						if (event.key.keysym.sym == SDLK_n)
						{
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
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
				exit(0);
				break;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					string s;
					stringstream ss;
					ss << score;
					ss >> s;
					renderImage("image/pause0.png");
					loadText("font/PressStart2P.ttf", 26, s, {255, 255, 255, 255}, 305, 90, 200, 40);
					bool quit3 = false;
					// Event handler
					SDL_Event e;
					int button = 0;
					// While application is running
					while (!quit3)
					{
						// Handle events on queue
						while (SDL_PollEvent(&e) != 0)
						{

							// User requests quit
							if (e.type == SDL_QUIT)
							{
								quit3 = true;
								exit(0);
								break;
							}

							// User presses a key
							else if (e.type == SDL_KEYDOWN)
							{

								if (e.key.keysym.sym == SDLK_ESCAPE)
								{
									quit3 = true;
									break;
								}
								if (e.key.keysym.sym == SDLK_UP && button == 1)
								{
									button = 0;
									renderImage("image/pause0.png");
									loadText("font/PressStart2P.ttf", 26, s, {255, 255, 255, 255}, 305, 90, 200, 40);
								}
								if (e.key.keysym.sym == SDLK_DOWN && button == 0)
								{
									button = 1;
									renderImage("image/pause1.png");
									loadText("font/PressStart2P.ttf", 26, s, {255, 255, 255, 255}, 305, 90, 200, 40);
								}
								if (e.key.keysym.sym == SDLK_RETURN && button == 1)
								{
									snakeBodyLength = 3;
									startmenu(0);
								}
								if (e.key.keysym.sym == SDLK_RETURN && button == 0)
								{
									quit3 = true;
									break;
								}
							}
						}
					}
				}

				// dua phan truoc ve vi tri sau
				for (int i = snakeBodyLength - 1; i > 0; i--)
				{
					snakeBody[i].x = snakeBody[i - 1].x;
					snakeBody[i].y = snakeBody[i - 1].y;
				}
				snakeBody[0].x = snakeHead.x;
				snakeBody[0].y = snakeHead.y;
				if (direction == LEFT)
				{
					if (e.key.keysym.sym == SDLK_DOWN)
					{
						direction = DOWN;
					}
					if (e.key.keysym.sym == SDLK_UP)
					{
						direction = UP;
					}
				}
				if (direction == RIGHT)
				{

					if (e.key.keysym.sym == SDLK_DOWN)
					{
						direction = DOWN;
					}
					if (e.key.keysym.sym == SDLK_UP)
					{
						direction = UP;
					}
				}
				if (direction == UP)
				{
					if (e.key.keysym.sym == SDLK_LEFT)
					{
						direction = LEFT;
					}
					// Tương tự với dịch phải, xuống và lên
					if (e.key.keysym.sym == SDLK_RIGHT)
					{
						direction = RIGHT;
					}
				}
				if (direction == DOWN)
				{
					if (e.key.keysym.sym == SDLK_LEFT)
					{
						direction = LEFT;
					}
					if (e.key.keysym.sym == SDLK_RIGHT)
					{
						direction = RIGHT;
					}
				}
				// neu an duoc diem
				if (snakeHead.x == point.x && snakeHead.y == point.y)
				{
					loadSound("sound/Move.wav");
					score = score + 10;
					// chuyển point ra chỗ khác
					point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
					point.y = (rand() % 48) * 10;
					// xu li diem o tren tuong
					bool pointOnWall = false;
					for (int j = 0; j < total_wall; j++)
					{
						if (point.x == wall[j].x && point.y == wall[j].y)
						{
							pointOnWall = true;
							break;
						}
					}
					while (pointOnWall)
					{
						bool check3 = true;
						point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
						point.y = (rand() % 48) * 10;
						for (int j = 0; j < total_wall; j++)
						{
							if (point.x == wall[j].x && point.y == wall[j].y)
							{
								check3 = false;
								break;
							}
						}
						if (check3)
						{
							pointOnWall = false;
						}
					}
					snakeBodyLength++;
					snakeBody[snakeBodyLength - 1].x = snakeBody[snakeBodyLength - 2].x;
					snakeBody[snakeBodyLength - 1].y = snakeBody[snakeBodyLength - 2].y;
				}
				// vẽ lại cái để ăn
				SDL_SetRenderDrawColor(gRenderer, 255, 51, 0, 0xFF);
				SDL_RenderFillRect(gRenderer, &point);
				if (snakeMoves(snakeHead, snakeBody, wall, point, snakeBodyLength, direction))
				{
					gameover();
					quit = true;
				}
			}
		}
	}
}

void loadSound(string path)
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Chunk *sound = Mix_LoadWAV(path.c_str());
	Mix_PlayChannel(-1, sound, 0);
}

void hscore(SDL_Event e)
{
	renderImage("highscore1.png");
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);
	ifstream myfile("highscore.txt");
	string line;
	if (myfile.is_open())
	{
		myfile >> highscore;
	}
	myfile.close();
	string s;
	stringstream ss;
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
			if (e.type == SDL_QUIT)
			{
				quit = true;
				exit(0);
				break;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if ((e.button.button == SDL_BUTTON_LEFT) && (e.button.x < 346 && e.button.x > 215 && e.button.y > 500 && e.button.y < 520))
				{
					startmenu(1);
					quit = true;
					break;
				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				startmenu(1);
				quit = true;
				break;
			}
		}
	}
}

void gameover()
{
	loadSound("sound/end.wav");
	if (score > highscore)
	{
		ofstream fout;
		fout.open("highscore.txt");
		fout << score;
		fout.close();
	}
	SDL_Event event;
	bool quit2 = false;
	while (!quit2)
	{

		//menu cu
		// renderImage("image/GameOver.png");
		// if (SDL_WaitEvent(&event))
		// {
		// 	if (event.type == SDL_QUIT)
		// 	{
		// 		quit2 = true;
		// 		exit(0);
		// 		break;
		// 	}
		// 	if (event.key.keysym.sym == SDLK_y)
		// 	{
		// 		playGame();
		// 	}
		// 	if (event.key.keysym.sym == SDLK_n)
		// 	{
		// 		quit2 = true;
		// 		exit(0);
		// 		break;
		// 	}
		// }
		string s;
		stringstream ss;
		ss << score;
		ss >> s;
		renderImage("image/gameover0.png");
		loadText("font/PressStart2P.ttf", 26, s, {255, 255, 255, 255}, 305, 90, 200, 40);
		bool quit3 = false;
		// Event handler
		SDL_Event e;
		int button = 0;
		// While application is running
		while (!quit3)
		{
			// Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{

				// User requests quit
				if (e.type == SDL_QUIT)
				{
					quit3 = true;
					exit(0);
					break;
				}

				// User presses a key
				else if (e.type == SDL_KEYDOWN)
				{

					if (e.key.keysym.sym == SDLK_ESCAPE)
					{
						quit3 = true;
						break;
					}
					if (e.key.keysym.sym == SDLK_UP && button == 1)
					{
						button = 0;
						renderImage("image/gameover0.png");
						loadText("font/PressStart2P.ttf", 26, s, {255, 255, 255, 255}, 305, 90, 200, 40);
					}
					if (e.key.keysym.sym == SDLK_DOWN && button == 0)
					{
						button = 1;
						renderImage("image/gameover1.png");
						loadText("font/PressStart2P.ttf", 26, s, {255, 255, 255, 255}, 305, 90, 200, 40);
					}
					if (e.key.keysym.sym == SDLK_RETURN && button == 1)
					{
						snakeBodyLength = 3;
						startmenu(0);
					}
					if (e.key.keysym.sym == SDLK_RETURN && button == 0)
					{
						snakeBodyLength = 3;
						playGame();
					}
				}
			}
		}
	}
}