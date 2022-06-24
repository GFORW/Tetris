#pragma once
#include <CnsFramework.h>
#include "Figure.h"
#include <random>
#include <conio.h>
#include <chrono>

#define MiddleX (unsigned int)((ScreenX-1)/2)
#define MiddleY (unsigned int)((ScreenY-1)/2)

#define ScorePanelEndX  (ScreenX-1)
#define ScorePanelStartX  (ScorePanelEndX - 10)
#define ScorePanelMiddleX (ScorePanelStartX + (ScorePanelEndX-ScorePanelStartX)/2)
#define ScorePanelMiddleY (unsigned int)((ScreenY-1)/2)

#define MiddleBoardX (1 + (unsigned int)(ScorePanelStartX/2))
#define MiddleBoardY (1 + (unsigned int)((ScreenY-2)/2))

#define min_tick 5

constexpr auto bounds = L'#';
constexpr auto space = L' ';
constexpr auto block = L'X';

class Game : public CnsFramework
{
public:
	Game(const int XSIZE = 30, const int YSIZE = 25, const std::chrono::nanoseconds tick = 30ms);
	~Game();
private:

	void KeyPressed(const int btnCode) const override;
	void Update() override;

	void drawFigure(Figure* const figptr);
	void clearFigure(Figure* const figptr);
	void drawScore(GameState* const state);
	
	void drawMenu();
	void drawTable();
	void drawFPS();

	void swap_pieces();
	void move();
	void GameOver();
	void force();

	void check_lines();
	void delete_lines();

	bool Collision();
	bool Handle_Events() override;

	int speed_count{};
	int SCORE{};

	std::shared_ptr<GameState> game;
	std::shared_ptr<GameState> menu;
	std::shared_ptr<GameState> game_over;

	std::vector<int> vLines;

	std::unique_ptr<Figure> ptrFigure;
	std::unique_ptr<Figure> ptrPreview;

	std::uint8_t env{ 0b000u };

	const COORD stPos{ (SHORT)MiddleBoardX, (SHORT)1 };
	const COORD previewPos{ (SHORT)(ScorePanelMiddleX - 1),(SHORT)2 };
};

enum ENV
{
	MENU = (1 << 0),
	GAME_OVER = (1 << 1),
	FORCE = (1 << 2)
};

namespace
{
	namespace RANDOM
	{
		std::random_device rd;
		std::seed_seq ss{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
		std::mt19937 rEng{ ss };

		inline int get_random(const int& min, const int& max)
		{
			std::uniform_int_distribution<int> distr1{ min, max };
			return distr1(rEng);
		};
	}
}