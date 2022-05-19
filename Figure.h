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
	down,
	blank
};

class Figure
{
public:
	Figure(fType type, COORD pos);
	Figure& operator=(const Figure& fig);
	~Figure();

	void RotateFigure();
	void SetBlock();

	std::wstring body;
	std::wstring tetromino[7];

	fType Type;

	COORD POS;

	int Rotate(int px, int py, int r);

	int rotation_counter = 0;

};
