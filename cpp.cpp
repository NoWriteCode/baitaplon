#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
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
int total_wall = 220;
int score = 0;

enum direction
{
	RIGHT,
	LEFT,
	UP,
	DOWN
};

// void snakeMove(SDL_Rect &snakeHead, SDL_Rect *snakeBody, SDL_Rect &point, int &snakeBodyLength, int direction);

// di chuyen con ran theo 1 huong xac dinh
void snakeMoves(SDL_Rect &snakeHead, SDL_Rect *snakeBody, SDL_Rect *wall, SDL_Rect &point, int &snakeBodyLength, int direction);

void renderImage(string path);

void loadSound(string path);

void playGame();

void loadText(const string &path, int font_size, const string &text, const SDL_Color color, int x, int y, int w, int h)
{
	TTF_Font *font = TTF_OpenFont(path.c_str(), font_size);
	auto textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	auto textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	SDL_Rect rect = {x, y, w, h};
	SDL_RenderCopy(gRenderer, textTexture, NULL, &rect);
	SDL_FreeSurface(textSurface);
	SDL_RenderPresent(gRenderer);
	SDL_DestroyTexture(textTexture);
}

int main(int argc, char *argv[])
{
	gWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	renderImage("image/StartMenu2.png");
	// SDL_Color textColor =
	TTF_Init();

	bool quit = false;

	// Event handler
	SDL_Event e;
	int a = 2;
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
				break;
			}

			// User presses a key
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_DOWN)
				{
					renderImage("image/StartMenu3.png");
					a = 3;
				}
				if (e.key.keysym.sym == SDLK_UP)
				{
					renderImage("image/StartMenu2.png");
					a = 2;
				}
				if (e.key.keysym.sym == SDLK_RETURN && a == 2)
				{
					loadSound("sound/StartGameSound.wav");
					playGame();
					quit = true;
				}
				if (e.key.keysym.sym == SDLK_RETURN && a == 3)
				{
					quit = true;
					break;
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				if (e.button.x < 500 && e.button.x > 90 && e.button.y > 300 && e.button.y < 330)
				{
					renderImage("image/StartMenu2.png");
					a = 2;
				}
				if (e.button.x < 430 && e.button.x > 160 && e.button.y > 355 && e.button.y < 382)
				{
					renderImage("image/StartMenu3.png");
					a = 3;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (a == 2)
					{
						loadSound("sound/StartGameSound.wav");
						playGame();
						quit = true;
					}
					if (a == 3)
					{
						quit = true;
						break;
					}
				}
			}
		}
	}

	return 0;
}

void snakeMoves(SDL_Rect &snakeHead, SDL_Rect *snakeBody, SDL_Rect *wall, SDL_Rect &point, int &snakeBodyLength, int direction)
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

	for (int i = 0; i < 3600; i++)
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
	loadText("font/PressStart2P.ttf", 15, "SCORE:" + s, {100, 100, 100, 255}, 50, 500, 200, 30);
	SDL_RenderPresent(gRenderer);
	SDL_Delay(100);
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
	SDL_Rect wall[3600];
	for (int i = 0; i < snakeMaxLength; i++)
	{
		snakeBody[i].w = 10;
		snakeBody[i].h = 10;
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
	for (int i = 0; i < 3600; i++)
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

	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		if (e.type == SDL_QUIT)
		{
			quit = true;
			break;
		}
		snakeMoves(snakeHead, snakeBody, wall, point, snakeBodyLength, direction);

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
							break;
						}
						if (event.key.keysym.sym == SDLK_y)
						{
							playGame();
						}
						if (event.key.keysym.sym == SDLK_n)
						{
							quit = true;
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
				break;
			}
			else if (e.type == SDL_KEYDOWN)
			{
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
				snakeMoves(snakeHead, snakeBody, wall, point, snakeBodyLength, direction);
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