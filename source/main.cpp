#include "snake.hpp"

int main(int argc, char *argv[])
{

	Snake* s = new Snake;
	s->loadSound("sound/PH.wav");
	s->startMenu(0);
	return 0;
}
