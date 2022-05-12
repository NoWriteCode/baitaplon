
#pragma once
#include <SDL2/SDL.h>
#include <string>

class Snake {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *screenSurface;

    public:
        Snake();
        ~Snake();
        int score;
        int highscore;
        enum direction{
            RIGHT,
            LEFT,
            UP,
            DOWN
        };
        static const int SCREEN_WIDTH = 600;
        static const int SCREEN_HEIGHT = 600;
        // static int total_wall;
        static const int step = 10;
        static int snakeBodyLength;
        SDL_Rect wall[220];

        void renderImage(std::string path);
        void loadSound(std::string path);
        void loadText(const std::string &font_path, int font_size, const std::string &text, const SDL_Color color, int x, int y, int w, int h);

        void startMenu(int button);
        void playGame();
        void drawPoint();
        int snakeMoves(SDL_Rect &snakeHead, SDL_Rect *snakeBody, SDL_Rect *wall, SDL_Rect &point, int &snakeBodyLength, int direction);
        void hscore(SDL_Event e);
        void gameOver();
};  