#include "Game.h"


Game::Game() : CnsFramework(30,25,50ms) // 
{
	game = GameState(ScreenX, ScreenY);
	game.name = "game";
	menu = GameState(ScreenX, ScreenY);
	menu.name = "menu";
	game_over = GameState(ScreenX, ScreenY);
	game_over.name = "game_over";
	win = GameState(ScreenX, ScreenY);
	win.name = "win";
	current_state = &menu;
	drawTable();
	drawMenu();
	dir = down;
	play_speed = 10;
	auto seed = std::chrono::system_clock::now();
	srand(std::chrono::system_clock::to_time_t(seed));
	speed_count = 0;
	ptrFigure = std::make_unique<Figure>((fType)(rand() % 7), stPos);
	ptrPreview = std::make_unique<Figure>((fType)(rand() % 7), prewPos);
	drawFigure(ptrPreview.get());

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
	else if ((btnCode == int('A')) || (btnCode == int('a')))
	{
		dir = left;
	}
	else if ((btnCode == int('D')) || (btnCode == int('d')))
	{
		dir = right;
	}
	else if ((btnCode == int('S')) || (btnCode == int('s')))
	{
		dir = down;
	}

}

bool Game::Handle_Events()
{
	if (current_state->name == "game_over")
	{
		if (GAME_OVER)
		{
			GAME_OVER = false;
			return true; // do another loop for render
		}
		std::cin.ignore();
		return false;
	}
	return true;
}

void Game::Update(GameState * State)
{
	drawFPS();
	if (State->name == "menu")
	{
		if (MENU)
		{
			MENU ^= MENU;
			return;
		}
		std::cin.ignore();
		MENU++;
		current_state = &game;
	}
	if (State->name == "game")
	{
		drawFPS();
		speed_count++;
		bForce = (speed_count == play_speed); // force piece down 
		clearFigure(ptrFigure.get());
		move();
		if (!vLines.empty())
			delete_lines();
		drawFigure(ptrFigure.get());
	}
	if (State->name == "game_over")
		GameOver();
}

void Game::clearFigure(Figure* figptr)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
			if (figptr->tetromino[figptr->Type][figptr->Rotate(px, py, figptr->rotation_counter)] == block)
				game.SetChar(figptr->POS.X + px, figptr->POS.Y + py, space);
}

void Game::drawFigure(Figure* figptr)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			if (figptr->tetromino[figptr->Type][figptr->Rotate(px, py, figptr->rotation_counter)] == block)
				game.SetChar(figptr->POS.X + px, figptr->POS.Y + py, block);
		}
}

void Game::move()
{
	old_coord = ptrFigure->POS;

	if (bForce)
	{
		force();
		return;
	}

	switch (dir)
	{
	case left:
		ptrFigure->POS.X -= 1;
		break;
	case right:
		ptrFigure->POS.X += 1;
		break;
	case down:
		ptrFigure->POS.Y += 1;
		break;
	case blank:
		break;
	}
	if (Collision())
		ptrFigure->POS = old_coord;


	if (dir == rotate)
	{
		ptrFigure->rotation_counter++;
		ptrFigure->rotation_counter %= 4;
		if (Collision())
			ptrFigure->rotation_counter--;
		else
			ptrFigure->RotateFigure();

	}

	dir = blank;

}

void Game::force()
{
	speed_count = 0;
	// Test if piece can be moved down
	ptrFigure->POS.Y += 1;
	if (Collision())
	{
		ptrFigure->POS = old_coord;
		drawFigure(ptrFigure.get());
		// lock piece 

		check_lines();
		swap_pieces();

		if (Collision())
		{
			GAME_OVER = true;
			current_state = &game_over;
		}
	}
	dir = blank;
}

void Game::delete_lines()
{
	for (auto &v : vLines)
		for (int px = 1; px < ScorePanelStartX ; px++)
		{
			for (int py = v; py > 1; py--)
			{
				game.SetChar(px, py, game.GetChar(px, py-1));
			}
		}
	SCORE += ScorePanelMiddleX - 1;
	if(play_speed > 1)
		play_speed -= 1;
	drawScore(&game);

	vLines.clear();
}

void Game::check_lines()
{
	for (int py = 0; py < 4; py++)
		if (ptrFigure->POS.Y + py < ScreenY-1)
		{
			bool bLine = true;
			for (int px = 1;  px < ScorePanelStartX; px++)
				bLine &= (game.GetChar((px), (ptrFigure->POS.Y + py)) != space);

			if (bLine)
			{
				// Remove Line, set to = ''
				for (int px = 1; px < ScorePanelStartX; px++)
					game.SetChar( px, (ptrFigure->POS.Y + py), space);
				vLines.push_back(ptrFigure->POS.Y + py);
			}
		}
}

void Game::swap_pieces()
{
	*ptrFigure = *ptrPreview;
	ptrFigure->POS = stPos;
	clearFigure(ptrPreview.get());

	auto seed = std::chrono::system_clock::now();
	srand(std::chrono::system_clock::to_time_t(seed));

	ptrPreview->Type = (fType)(rand() % 7);
	ptrPreview->SetBlock();
	ptrPreview->POS = prewPos;
	drawFigure(ptrPreview.get());

	dir = down;
}

bool Game::Collision()
{ 
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = ptrFigure->Rotate(px, py, ptrFigure->rotation_counter);

			if (ptrFigure->tetromino[ptrFigure->Type][pi] == block)
			{
				if ((ptrFigure->POS.X + px >= 0 && ptrFigure->POS.X + px <= ScorePanelStartX) && (ptrFigure->POS.Y + py >= 0 && ptrFigure->POS.Y + py <= ScreenY))
				{
					if (game.GetChar((ptrFigure->POS.X + px), (ptrFigure->POS.Y + py)) != space)
						return true;
				}
			}
		}
	return false;
}

void Game::GameOver()
{
	std::string gm_ov = "GAME OVER";
	for (unsigned int i = 0, x = MiddleBoardX - gm_ov.size() / 2; i < gm_ov.size(); x++, i++)
	{
		game_over.SetChar(x, MiddleBoardY, gm_ov.at(i));
	}

	std::string score = "SCORE";
	for (size_t i = 0, x = ScorePanelMiddleX - score.size() / 2; i < score.size(); x++, i++)
	{
		game_over.SetChar(x, ScorePanelMiddleY, score.at(i));
	}
	drawScore(&game_over);
};

void Game::drawTable()
{

	// fill bounds
	for (int y = 0; y < ScreenY; y++)
	{
		for (int x = 0; x < ScreenX; x++)
		{
			if ((y == 0) || (x == 0) || x == ScreenX - 1 || y == ScreenY - 1)
				game.SetChar(x, y, bounds);
			if (x == ScorePanelStartX)
				game.SetChar(x, y, bounds);
		}
	}
	std::string score = "SCORE";
	for (size_t i = 0, x = ScorePanelMiddleX - score.size() / 2; i < score.size(); x++, i++)
	{
		game.SetChar(x, ScorePanelMiddleY, score.at(i));
	}
	drawScore(&game);
}

void Game::drawScore(GameState * state)
{
	std::wstring scr = std::to_wstring(SCORE);
	for (unsigned int i = 0, x = ScorePanelMiddleX; i < scr.size(); x++, i++)
	{
		state->SetChar(x, ScorePanelMiddleY + 1, scr[i]);
	}
}

void Game::drawFPS()
{
	std::string title = "Tetris FPS: ";
	title += std::to_string(FPS);
	SetConsoleTitleA(title.c_str());
}

void Game::drawMenu()
{

	std::string text = "TETRIS GAME";
	std::string text1 = " - INFINITE GAME";
	std::string text2 = " - DO NOT FILL THE SCREEN WITH PARTS";
	std::string text3 = " - EACH FULL ROW WILL GET DELETED";
	std::string text4 = "PRESS ANY BUTTON TO START";

	if (ScreenX < text4.size() * 2)
	{
		current_state = &game;
		return;
	}

	for (size_t i = 0, x = MiddleX - text.size() /2 ; i < text.size(); x++, i++)
	{
		menu.SetChar(x, MiddleY -5, text.at(i));
	}
	for (size_t i = 0, x = MiddleX - (text2.size() / 2) ; i < text1.size(); x++, i++)
	{
		menu.SetChar(x, MiddleY , text1.at(i));
	}
	for (size_t i = 0, x = MiddleX - (text2.size() / 2);  i < text2.size(); x++, i++)
	{
		menu.SetChar(x, MiddleY + 1, text2.at(i));
	}
	for (size_t i = 0, x = MiddleX - (text2.size()/2) ; i < text3.size(); x++, i++)
	{
		menu.SetChar(x, MiddleY + 2, text3.at(i));
	}

	for (size_t i = 0, x = MiddleX - text4.size() / 2; i < text4.size(); x++, i++)
	{
		menu.SetChar(x, ScreenY -2, text4.at(i));
	}
	MENU = true;
}
