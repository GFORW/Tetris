#include "Game.h"


Game::Game() : Engine(50,25) // 
{
	GameSpeed = 500;
	play = TRUE;
	drawMenu();
	dir = down;
	COORD prewPos{ ScorePanelMiddleX - 1, 3 };
	srand(time(0));
	ptrFigure = std::make_unique<Figure>((fType)(rand() % 7), stPos);
	ptrPreview= std::make_unique<Figure>((fType)(rand() % 7), prewPos);
}

Game::~Game()
{
}

void Game::KeyPressed(int btnCode)
{
	if ((btnCode == 32) ) // space bar
	{
		dir = rotate;
	}
	else if ((btnCode == int('A')) || (btnCode == int('a')) || (btnCode == 77)) //a <-
	{
		dir = left;
	}
	else if ((btnCode == int('D')) || (btnCode == int('d')) || (btnCode == 224)) //d
	{
		dir = right;
	}
}

void Game::Update()
{
	drawFPS();
	if (GAME_OVER)
	{
		GameOver();
		drawScore();
		return;
	}
	if (MENU == 1)
	{
		MENU++;
		return;
	}
	else if (MENU == 2)
	{
		while(!_kbhit())
		MENU++;
		for (int i = 0; i < ScreenX; i++)
			for (int j = 0; j < ScreenY; j++)
				SetChar(i, j, L' ');
		drawTable();
		drawFigure(ptrPreview.get());
	}
	clearFigure(ptrFigure.get());
	move();
	drawFigure(ptrFigure.get());
}

void Game::drawTable()
{

	// fill bounds
	for (int y = 0; y < ScreenY; y++)
	{
		for (int x = 0; x < ScreenX; x++)
		{
			if ((y == 0) || (x == 0) || x == ScreenX-1 || y == ScreenY-1)
				SetChar(x, y, bounds);
			if (x == ScorePanelStartX)  
				SetChar(x, y, bounds);
		}
	}
	std::string score = "SCORE";
	for (size_t i=0, x = ScorePanelMiddleX -score.size()/2; i < score.size();x++,i++)
	{
			SetChar(x, ScorePanelMiddleY, score.at(i));
	}
	drawScore();
}

void Game::drawScore()
{
	std::wstring scr = std::to_wstring(SCORE);
	for (unsigned int i = 0, x = ScorePanelMiddleX; i < scr.size(); x++, i++)
	{
		SetChar(x, ScorePanelMiddleY+1, scr[i]);
	}
}

void Game::clearFigure(Figure* figptr)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
			if (figptr->body[figptr->Rotate(px, py, figptr->rotation_counter)] != L'.')
				SetChar(figptr->POS.X + px, figptr->POS.Y + py, space);
}

void Game::drawFigure(Figure* figptr)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			if (figptr->body[figptr->Rotate(px, py, figptr->rotation_counter)] != L'.')
				SetChar(figptr->POS.X + px, figptr->POS.Y + py, block);
		}
}

void Game::move()  
{
	COORD old_coord = ptrFigure->POS;
	int old_r = ptrFigure->rotation_counter;
	switch (dir)
	{
	case rotate:
		ptrFigure->rotation_counter++;
		ptrFigure->rotation_counter %= 4;
		ptrFigure->RotateFigure();
		if (Collision())
			ptrFigure->rotation_counter = old_r;
		dir = down;
		return;
		break;
	case left:
		ptrFigure->POS.X -= 1;
		break;
	case right:
		ptrFigure->POS.X += 1;
		break;
	case down:
		ptrFigure->POS.Y += 1;
		break;
	}
	place = 1;
	if (Collision())
	{
		ptrFigure->POS = old_coord;
		dir = down;
		return;
	}
	dir = down;
}

void Game::drawFPS()
{
	std::string title = "Tetris      FPS : ";
	title += std::to_string(FPS);
	SetConsoleTitleA(title.c_str());
}

void Game::lock()
{
	COORD prewPos{ ScorePanelMiddleX-1, 1};

	drawFigure(ptrFigure.get());
	ptrFigure->Type = ptrPreview->Type;
	ptrFigure->SetBlock();
	ptrFigure->POS = stPos;

	clearFigure(ptrPreview.get());
	srand(time(0));
	ptrPreview->Type = (fType)(rand() % 7);
	ptrPreview->SetBlock();
	ptrPreview->POS = prewPos;
	drawFigure(ptrPreview.get());
}

bool Game::Collision()
{ 
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = ptrFigure->Rotate(px, py, ptrFigure->rotation_counter);
			//int pi = py * 4 + px;
			if (ptrFigure->POS.X + px > 0 && ptrFigure->POS.X + px < ScorePanelStartX+1)
			{
				if (ptrFigure->POS.Y + py > 0)
					if(ptrFigure->POS.Y + py < ScreenY-1)
					{
						// In Bounds, so do collision check
						if (ptrFigure->body[pi] == block && GetChar((ptrFigure->POS.X + px), (ptrFigure->POS.Y + py)) == bounds)
							return true;
						if (ptrFigure->body[pi] == block && GetChar((ptrFigure->POS.X + px), (ptrFigure->POS.Y + py)) == block)
						{
							if (ptrFigure->body[pi] == block && GetChar((stPos.X + px), (stPos.Y + py)) == block) //start blocked
							{
								GAME_OVER = true;
								return true;
							}
							if (place)
							{
								lock();
								place = 0;
								return false;
							}
							return true;
						}
					} 
					else
					{
						if (place)
						{
							lock();
							place = 0;
							return false;
						}
						return true;
					}
			}
			else 
				return true;
		}
	return false;
}

void Game::drawMenu()
{
	std::string text = "TETRIS GAME";
	std::string text1 = " - INFINITE GAME";
	std::string text2 = " - DO NOT FILL THE SCREEN WITH PARTS";
	std::string text3 = " - EACH FULL ROW WILL GET DELETED";
	std::string text4 = "PRESS ANY BUTTON TO START";

	for (size_t i = 0, x = MiddleX - text.size() /2 ; i < text.size(); x++, i++)
	{
		SetChar(x, MiddleY -5, text.at(i));
	}
	for (size_t i = 0, x = MiddleX - (text2.size() / 2) ; i < text1.size(); x++, i++)
	{
		SetChar(x, MiddleY , text1.at(i));
	}
	for (size_t i = 0, x = MiddleX - (text2.size() / 2);  i < text2.size(); x++, i++)
	{
		SetChar(x, MiddleY + 1, text2.at(i));
	}
	for (size_t i = 0, x = MiddleX - (text2.size()/2) ; i < text3.size(); x++, i++)
	{
		SetChar(x, MiddleY + 2, text3.at(i));
	}

	for (size_t i = 0, x = MiddleX - text4.size() / 2; i < text4.size(); x++, i++)
	{
		SetChar(x, ScreenY -2, text4.at(i));
	}
	MENU++;
}

void Game::GameOver()
{
	play = FALSE;
	std::string gm_ov = "GAME OVER";
	for (unsigned int i = 0, x = MiddleBoardX - gm_ov.size()/2; i < gm_ov.size(); x++, i++)
	{
		SetChar(x, MiddleBoardY, gm_ov.at(i));
	}
};
