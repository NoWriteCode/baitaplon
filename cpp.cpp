#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <cstdlib>
#include <ctime>
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
int total_wall = 120;

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

void playGame();

int main(int argc, char *args[])
{
	gWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

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
				break;
			}

			// User presses a key
			else if (e.type == SDL_KEYDOWN)
			{
				playGame();
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				playGame();
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
	for (int i = 0; i < total_wall; i++)
	{
		if (i < total_wall / 2)
		{
			wall[i].x = 0;
			wall[i].y = i * 10;
		}
		else
		{
			wall[i].x = SCREEN_WIDTH - 10;
			wall[i].y = (i - SCREEN_WIDTH / 10) * 10;
		}
	}
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
	for (int i = 0; i < 120; i++)
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
		// chuyển point ra chỗ khác
		point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
		point.y = (rand() % (SCREEN_WIDTH / 10)) * 10;
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
			point.y = (rand() % (SCREEN_WIDTH / 10)) * 10;
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

	SDL_RenderPresent(gRenderer);
	SDL_Delay(50);
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
	for (int i = 0; i < total_wall; i++)
	{
		if (i < total_wall / 2)
		{
			wall[i].x = 0;
			wall[i].y = i * 10;
		}
		else
		{
			wall[i].x = SCREEN_WIDTH - 10;
			wall[i].y = (i - SCREEN_WIDTH / 10) * 10;
		}
	}
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
	for (int i = 0; i < 120; i++)
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
	point.y = (rand() % (SCREEN_WIDTH / 10)) * 10;
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
		point.y = (rand() % (SCREEN_WIDTH / 10)) * 10;
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
					// chuyển point ra chỗ khác
					point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
					point.y = (rand() % (SCREEN_WIDTH / 10)) * 10;
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
						point.y = (rand() % (SCREEN_WIDTH / 10)) * 10;
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
