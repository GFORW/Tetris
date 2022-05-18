#pragma once
#include "Engine.h"
#include "Figure.h"
#include <random>
#include <time.h>
#include <conio.h>


#define MiddleX (unsigned int)((ScreenX-1)/2)
#define MiddleY (unsigned int)((ScreenY-1)/2)

#define ScorePanelEndX  (ScreenX-1)
#define ScorePanelStartX  (ScorePanelEndX - 10)
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
private:

	virtual void KeyPressed(int btnCode);
	virtual void Update();

	bool Collision();

	void drawFigure(Figure* figptr);
	void clearFigure(Figure* figptr);

	void drawMenu();
	void drawTable();
	void drawScore();
	void drawFPS();

	void swap_pieces();
	void move();
	void gameover();
	void check_lines();
	void delete_lines();
	void force();
	Direction dir;

	int speed_count;
	int MENU = 0;
	int SCORE = 0;

	int play_speed;

	std::vector<int> vLines;
	COORD old_coord;

	bool GAME_OVER = FALSE;
	bool bForce;

	std::unique_ptr<Figure> ptrFigure;
	std::unique_ptr<Figure> ptrPreview;

	COORD stPos{ (SHORT)MiddleBoardX, (SHORT)1 };
	COORD prewPos{ (SHORT) (ScorePanelMiddleX - 1),(SHORT)2 };
};