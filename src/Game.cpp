#include "Game.h"


Game::Game(const int XSIZE, const int YSIZE, const std::chrono::nanoseconds tick) : CnsFramework(XSIZE,YSIZE,tick) // 
{
	game = std::make_shared<GameState>(ScreenX, ScreenY, "game"); 
	menu = std::make_shared<GameState>(ScreenX, ScreenY, "menu");
	game_over = std::make_shared<GameState>(ScreenX, ScreenY, "game_over");

	current_state = menu.get();
	drawTable();
	drawMenu();
	
	ptrFigure = std::make_unique<Figure>((fType)(RANDOM::get_random(0,6)), stPos);
	ptrPreview = std::make_unique<Figure>((fType)(RANDOM::get_random(0, 6)), previewPos);

	drawFigure(ptrPreview.get());
}

Game::~Game()
{
}

void Game::KeyPressed(const int btnCode) const 
{
	if (btnCode == 32) // space bar
	{
		ptrFigure->dir = rotate;
	}
	else if ((btnCode == int('A')) || (btnCode == int('a')))
	{
		ptrFigure->dir = left;
	}
	else if ((btnCode == int('D')) || (btnCode == int('d')))
	{
		ptrFigure->dir = right;
	}
	else if ((btnCode == int('S')) || (btnCode == int('s')))
	{
		ptrFigure->dir = down;
	}

}

bool Game::Handle_Events()
{
	if (current_state->name == "game_over")
	{
		if (env & GAME_OVER)
		{
			std::cin.ignore();
			return false;
		}
		else
			env |= GAME_OVER;
	}
	return true;
}

void Game::Update()
{
	drawFPS();
	if (current_state->name == "menu")
	{
		if (env & MENU)
		{
			std::cin.ignore();
			current_state = game.get();
		}
		else env |= MENU;

		return;
	}
	if (current_state->name == "game")
	{
		{
			speed_count++;

			const int mod = SCORE % (ScorePanelStartX - 2);

			auto legit_speed{
				[this,mod]()->int {
				const int loc_tick = std::chrono::duration_cast<std::chrono::milliseconds>(tick).count();
				const int res = loc_tick - mod;
				if ((loc_tick - mod) > min_tick)
					return loc_tick - mod;
				else 
					return min_tick; }
			};

			if (speed_count == legit_speed())// force piece down 
			{
				env |= FORCE;
				speed_count = 0;
			}
		}
	
		clearFigure(ptrFigure.get());
		move();
		if (!vLines.empty())
			delete_lines();
		drawFigure(ptrFigure.get());
	}
	if (current_state->name == "game_over")
		GameOver();

	drawScore(current_state);
}

void Game::clearFigure(Figure* const figptr)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
			if(tetromino[figptr->Type][figptr->Rotate(px, py, figptr->rotation_counter)] == block)
				game->SetChar(figptr->POS.X + px, figptr->POS.Y + py, space);
}

void Game::drawFigure(Figure* const figptr)
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			if(tetromino[figptr->Type][figptr->Rotate(px, py, figptr->rotation_counter)] == block)
				game->SetChar(figptr->POS.X + px, figptr->POS.Y + py, block);
		}
}

void Game::move()
{
	ptrFigure->OLD_POS = ptrFigure->POS;

	if (env & FORCE)
	{
		force();
		env &= ~FORCE;
		return;
	}

	switch (ptrFigure->dir)
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
	{
		ptrFigure->POS = ptrFigure->OLD_POS;
		if (ptrFigure->dir == down)
			force();
	}


	if (ptrFigure->dir == rotate)
	{
		ptrFigure->rotation_counter++;
		ptrFigure->rotation_counter %= 4;
		if (Collision())
			ptrFigure->rotation_counter--;
		else
			ptrFigure->RotateFigure();

	}

	ptrFigure->dir = blank;

}

void Game::force()
{
	// Test if piece can be moved down
	ptrFigure->POS.Y += 1;
	if (Collision())
	{
		ptrFigure->POS = ptrFigure->OLD_POS;
		drawFigure(ptrFigure.get());
		// lock piece 
		check_lines();
		swap_pieces();

		if (Collision())
		{
			env |= GAME_OVER;
			current_state = game_over.get();
		}
	}
	ptrFigure->dir = blank;
}

void Game::delete_lines()
{
	for (auto &v : vLines)
		for (int px = 1; px < ScorePanelStartX ; px++)
		{
			for (int py = v; py > 1; py--)
			{
				game->SetChar(px, py, game->GetChar(px, py-1));
			}
		}
	SCORE += ScorePanelStartX - 2;
	drawScore(game.get());

	vLines.clear();
}

void Game::check_lines()
{
	for (int py = 0; py < 4; py++)
		if (ptrFigure->POS.Y + py < ScreenY-1)
		{
			bool bLine = true;
			for (int px = 1;  px < ScorePanelStartX; px++)
				bLine &= (game->GetChar((px), (ptrFigure->POS.Y + py)) != space);

			if (bLine)
			{
				// Remove Line, set to = ''
				for (int px = 1; px < ScorePanelStartX; px++)
					game->SetChar( px, (ptrFigure->POS.Y + py), space);
				vLines.push_back(ptrFigure->POS.Y + py);
			}
		}
}

void Game::swap_pieces()
{
	*ptrFigure = *ptrPreview;
	ptrFigure->POS = stPos;
	ptrFigure->dir = down;

	clearFigure(ptrPreview.get());
	
	ptrPreview->Type = (fType)(RANDOM::get_random(0,6));
	ptrPreview->SetBlock();
	
	drawFigure(ptrPreview.get());
}

bool Game::Collision()
{ 
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = ptrFigure->Rotate(px, py, ptrFigure->rotation_counter);

			if (tetromino[ptrFigure->Type][pi] == block)
			{
				if ((ptrFigure->POS.X + px >= 0 && ptrFigure->POS.X + px <= ScorePanelStartX) && (ptrFigure->POS.Y + py >= 0 && ptrFigure->POS.Y + py <= ScreenY))
				{
					if (game->GetChar((ptrFigure->POS.X + px), (ptrFigure->POS.Y + py)) != space)
						return true;
				}
			}
		}
	return false;
}

void Game::GameOver()
{
	const std::string& const gm_ov = "GAME OVER";
	for (unsigned int i = 0, x = MiddleBoardX - gm_ov.size() / 2; i < gm_ov.size(); x++, i++)
	{
		game_over->SetChar(x, MiddleBoardY, gm_ov.at(i));
	}
};

void Game::drawTable()
{

	// fill bounds
	for (int y = 0; y < ScreenY; y++)
	{
		for (int x = 0; x < ScreenX; x++)
		{
			if ((y == 0) || (x == 0) || x == ScreenX - 1 || y == ScreenY - 1)
				game->SetChar(x, y, bounds);
			if (x == ScorePanelStartX)
				game->SetChar(x, y, bounds);
		}
	}
}

void Game::drawScore(GameState * const state)
{
	const std::string& const score = "SCORE";
	for (unsigned int i = 0, x = ScorePanelMiddleX - score.size() / 2; i < score.size(); x++, i++)
	{
		state->SetChar(x, ScorePanelMiddleY, score.at(i));
	}

	const std::wstring& const scr =  std::to_wstring(SCORE);
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

	const std::string& const text  = "TETRIS GAME";
	const std::string& const text1 = " - INFINITE GAME";
	const std::string& const text2 = " - DO NOT FILL THE SCREEN WITH PARTS";
	const std::string& const text3 = " - EACH FULL ROW WILL GET DELETED";
	const std::string& const text4 = "PRESS ANY BUTTON TO START";

	if (ScreenX < text4.size() * 2)
	{
		current_state = game.get();
		return;
	}

	for (unsigned int i = 0, x = MiddleX - text.size() /2 ; i < text.size(); x++, i++)
	{
		menu->SetChar(x, MiddleY -5, text.at(i));
	}
	for (unsigned int i = 0, x = MiddleX - (text2.size() / 2) ; i < text1.size(); x++, i++)
	{
		menu->SetChar(x, MiddleY , text1.at(i));
	}
	for (unsigned int i = 0, x = MiddleX - (text2.size() / 2);  i < text2.size(); x++, i++)
	{
		menu->SetChar(x, MiddleY + 1, text2.at(i));
	}
	for (unsigned int i = 0, x = MiddleX - (text2.size()/2) ; i < text3.size(); x++, i++)
	{
		menu->SetChar(x, MiddleY + 2, text3.at(i));
	}

	for (unsigned int i = 0,  x = MiddleX - text4.size() / 2; i < text4.size(); x++, i++)
	{
		menu->SetChar(x, ScreenY -2, text4.at(i));
	}
}
