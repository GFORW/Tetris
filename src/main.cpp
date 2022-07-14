#include "Game.h"

int main()
{
	Game* const Tetris = new Game();
	Tetris->Run();
	delete Tetris;
	return 0;
}
