#include "snake.hpp"
#include <SDL2/SDL_mixer.h>

void Snake::loadSound(std::string path)
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Chunk *sound = Mix_LoadWAV(path.c_str());
	Mix_PlayChannel(-1, sound, 0);
}