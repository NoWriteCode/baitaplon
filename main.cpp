#include <bits/stdc++.h>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 720;
const int RECT_SIZE = 20;
const int STEP = 20;
const std::string WINDOW_TITLE = "An Implementation of Code.org Painter";

void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer) 
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    //Khi chạy trong môi trường bình thường (không chạy trong máy ảo)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy ảo (ví dụ tại máy tính trong phòng thực hành ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(50);
    }
}

void draw(SDL_Renderer* renderer, int x, int y) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.h = RECT_SIZE;
	rect.w = RECT_SIZE;
	SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);
}

void clearScreen(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 24, 64, 255);
	SDL_RenderClear(renderer);
}

enum Direction {
	UP, DOWN, LEFT, RIGHT, QUIT
};

const int x_STEP[] = {0, 0, -1, 1};
const int y_STEP[] = {-1, 1, 0, 0};
void updateGame(int& x, int& y, Direction event) {
	x += x_STEP[event] * STEP;
	y += y_STEP[event] * STEP;
}

void getEvent(Direction& event) {
	SDL_Event e;
		if (SDL_PollEvent(&e) == 0) {}
		else if (e.type == SDL_QUIT) event = QUIT;
		else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym)
			{
			case SDLK_UP: 
			case SDLK_w: event = UP; break;
			case SDLK_DOWN: 
			case SDLK_s: event = DOWN; break;
			case SDLK_LEFT: 
			case SDLK_a: event = LEFT; break;  
			case SDLK_RIGHT: 
			case SDLK_d: event = RIGHT; break;
			}
		}
}

bool gameOver(int x, int y) {
	return x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT;
}

int main(int argc, char* argv[]) 
{
	SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);

	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 2;

	SDL_Event e;
	Direction event = UP;
	while (!gameOver(x, y)) {
		clearScreen(renderer);
		getEvent(event);
		if (event == QUIT) break;
		updateGame(x, y, event);
		draw(renderer, x, y);
		SDL_Delay(50);
	}

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}