#pragma once
#include <CnsFramework.h>
#include "Figure.h"
#include <random>
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

class Game : public CnsFramework
{
public:
	Game();
	~Game();
private:

	void  KeyPressed  (int btnCode) override;
	void  Update (GameState * State) override;

	void drawFigure(Figure* figptr);
	void clearFigure(Figure* figptr);
	void drawScore(GameState * State);
	
	void drawMenu();
	void drawTable();
	void drawFPS();
	bool Handle_Events();

	void swap_pieces();
	void move();
	void GameOver();
	void force();

	void check_lines();
	void delete_lines();

	bool Collision();

	Direction dir;

	int speed_count;
	int SCORE = 0;

	int play_speed;

	GameState game;
	GameState menu;
	GameState game_over;
	GameState win;

	std::vector<int> vLines;
	COORD old_coord;

	bool GAME_OVER = false;
	bool bForce;
	bool MENU = false;

	std::unique_ptr<Figure> ptrFigure;
	std::unique_ptr<Figure> ptrPreview;

	COORD stPos{ (SHORT)MiddleBoardX, (SHORT)1 };
	COORD prewPos{ (SHORT) (ScorePanelMiddleX - 1),(SHORT)2 };
};