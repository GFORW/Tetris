#pragma once
#include "Engine.h"
#include "Figure.h"
#include <random>
#include <time.h>
#include <conio.h>


#define MiddleX (unsigned int)((ScreenX-1)/2)
#define MiddleY (unsigned int)((ScreenY-1)/2)

#define ScorePanelEndX  (ScreenX-1)
#define ScorePanelStartX  (ScorePanelEndX - 11)
#define ScorePanelMiddleX (ScorePanelStartX + (ScorePanelEndX-ScorePanelStartX)/2)
#define ScorePanelMiddleY (unsigned int)((ScreenY-1)/2)

#define MiddleBoardX (1 + (unsigned int)(ScorePanelStartX/2))
#define MiddleBoardY (1 + (unsigned int)((ScreenY-2)/2))

#define bounds L'#'
#define space L' '
#define block L'X'

class Game : public Engine
{
public:
	Game();
	~Game();

	virtual void KeyPressed(int btnCode);
	virtual void Update();

	bool Collision();

	void drawMenu();
	void drawTable();
	void drawScore();
	void drawFigure(Figure* figptr);
	void drawFPS();
	void clearFigure(Figure* figptr);
	void lock();
	void move();
	void GameOver();

	std::unique_ptr<Figure> ptrFigure;
	std::unique_ptr<Figure> ptrPreview;
	Direction dir;

private:
	int place = 0;
	int MENU = 0;
	bool GAME_OVER = FALSE;
	int SCORE = 0;
	COORD stPos{ (SHORT)MiddleBoardX, (SHORT)1 };
};