#pragma once
#include <windows.h>
#include <iostream>
enum fType
{
	I,
	T,
	O,
	S,
	Z,
	J,
	L
};

enum Direction
{
	left,
	rotate,
	right,
	down
};

class Figure
{
public:
	Figure(fType type, COORD pos);
	~Figure();
	void RotateFigure();
	void SetBlock();
	std::wstring body;
	fType Type;
	int Rotate(int px, int py, int r);
	COORD POS;
	int rotation_counter = 0;
private:
	std::wstring tetromino[7];
};
