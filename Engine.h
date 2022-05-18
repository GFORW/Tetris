#pragma once
#include <Windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <iostream>


class Engine
{
public:
	Engine(int X = 50, int Y = 25, int tick_msc = 50);
	virtual ~Engine();


	virtual void KeyPressed(int Code) {};
	virtual void Update() {};


	void SetChar(unsigned int x, unsigned int y, wchar_t c) ;
	void Run();
	wchar_t GetChar(unsigned int x, unsigned int y);
	void ChangeTickSpeed(int msec);


	int ScreenX;
	int ScreenY;

	int tick;
	int play;
	int FPS;

private:
	void Render();

	std::vector<std::vector<wchar_t>> Screen;

	HANDLE hConsole;
	HANDLE hConsoleIn;

	COORD windowBufSize;
	COORD DwBufferCoord;

	SMALL_RECT WindowSize;
};