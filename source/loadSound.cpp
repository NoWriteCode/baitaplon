#include "snake.hpp"
#include <SDL2/SDL_mixer.h>

void Snake::loadSound(std::string path) {

	//kno loop
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Chunk *sound = Mix_LoadWAV(path.c_str());
	Mix_PlayChannel(-1, sound, 0);

	//loop
	// Mix_Music *music = NULL;
	// Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
	// music = Mix_LoadMUS(path.c_str());
	// Mix_PlayMusic(music, -1);
}
