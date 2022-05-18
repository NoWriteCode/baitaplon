#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 

#include <string>

class Snake {
    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Surface *screenSurface;
        SDL_Surface *hello;

    public:
        Snake();
        ~Snake();

        enum direction {
            RIGHT,
            LEFT,
            UP,
            DOWN
        };

        int score;
        int highscore;

        static const int SCREEN_WIDTH  = 600;
        static const int SCREEN_HEIGHT = 600;
        static const int step          =  10;
        static int snakeBodyLength;

        SDL_Rect wall[261];

        void renderImage(std::string path);
        void loadSound(std::string path, int x);
        void loadText(const std::string &font_path, int font_size, const std::string &text, const SDL_Color color, int x, int y, int w, int h);

        void startMenu(int button);
        void playGame();

        void drawPoint(SDL_Rect &point);
        void drawMap();

        int snakeMoves(SDL_Rect &snakeHead, SDL_Rect *snakeBody, SDL_Rect *wall, SDL_Rect &point, int &snakeBodyLength, int direction);
        void highScore(SDL_Event e);
        void gameOver();
};  