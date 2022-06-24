#include "Game.h"

int main()
{
	Game* Tetris = new Game();
	Tetris->Run();
	delete Tetris;
	return 0;
}
