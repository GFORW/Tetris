#pragma once
#include "Engine.h"
#include <conio.h>

Engine::Engine(int X, int Y) : ScreenX(X), ScreenY(Y)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	// disable text selection
	{
		DWORD mode;
		GetConsoleMode(hConsoleIn, &mode);
		mode &= ENABLE_EXTENDED_FLAGS;
		if (!SetConsoleMode(hConsoleIn, mode))
		{
			std::cout << "SetConsoleMode failed" << GetLastError() << std::endl;
		}
	}
	// set font 
	{
		CONSOLE_FONT_INFOEX ConsoleFontInfo;
		GetCurrentConsoleFontEx(hConsole, FALSE , &ConsoleFontInfo); 
		ConsoleFontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		ConsoleFontInfo.FontWeight = 400;
		ConsoleFontInfo.FontFamily = 72;
		wcscpy_s(ConsoleFontInfo.FaceName, L"Terminal");
		ConsoleFontInfo.dwFontSize = COORD{ 8,12 };
		SetCurrentConsoleFontEx(hConsole, FALSE, &ConsoleFontInfo);
	}

	// turn off cursor
	{
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = 0;
		cursorInfo.dwSize = 1;
		if (!SetConsoleCursorInfo(hConsole, &cursorInfo))
		{
			std::cout << "SetConsoleCurInfo failed" << GetLastError() << std::endl;
		}
	}


	WindowSize = { 0, 0, 1, 1 };    // BECAUSE WINDOWS IS STUPID
	SetConsoleWindowInfo(hConsole, TRUE, &WindowSize);

	windowBufSize = {(short)(ScreenX),(short)(ScreenY)};
	if (!SetConsoleScreenBufferSize(hConsole, windowBufSize))
	{
		std::cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << std::endl;
		std::cin.ignore();
	}

	WindowSize = { 0, 0, (short)(ScreenX - 1), (short)(ScreenY- 1)};
	if (!SetConsoleWindowInfo(hConsole, TRUE, &WindowSize))
	{
		std::cout << "SetConsoleWindowInfo failed with error " << GetLastError() << std::endl;
		std::cin.ignore();
	}

	//prevent window resizing
	HWND cWin = GetConsoleWindow();
	SetWindowLong(cWin, GWL_STYLE, GetWindowLong(cWin, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	// fill up array
	{
		Screen.resize(ScreenX, std::vector<wchar_t>(ScreenY, L' '));

		for (int i = 0; i < ScreenX; i++)
			for (int j = 0; j < ScreenY; j++)
				Screen.at(i).at(j) = L' ';
	}
}

Engine::~Engine()
{

}

// Draw Screen
void Engine::Render()
{
	DWORD Chars;
	COORD pos;
	for (short i = 0; i < ScreenX; i++)
		for (short j = 0; j < ScreenY; j++)
		{
			pos = { i,j };
			FillConsoleOutputCharacter(hConsole, Screen.at(i).at(j), 1, pos, &Chars);
		}
}

void Engine::SetChar(unsigned int x, unsigned int y, wchar_t c)
{
	Screen.at(x).at(y) = c;
}

wchar_t Engine::GetChar(unsigned int x, unsigned int y)
{
	return Screen.at(x).at(y);
}

void Engine::Run()
{
	std::chrono::high_resolution_clock timer;
	while(play)
	{
		auto start = timer.now();
		if (_kbhit())
		{
			KeyPressed(_getch());
			if (!FlushConsoleInputBuffer(hConsoleIn))
				std::cout << "FlushConsoleInputBuffer failed with error " << GetLastError();
		}
		Update();
		Render();

		auto end = timer.now();
		auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		if (delta > 0)
			FPS = ( 1.00f / delta)* 1000.0f;
		Sleep(GameSpeed);
	}
	std::cin.ignore();
}

void Engine::ChangeSpeed(int sec)
{
	GameSpeed = sec;
}