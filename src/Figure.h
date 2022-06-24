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


const std::wstring tetromino[7]{ L"..X...X...X...X.",
								 L"..X..XX...X.....",
							     L".....XX..XX.....",
							     L"..X..XX..X......",
								 L".X...XX...X.....",
								 L"..X...X..XX.....",
								 L".X...X...XX....." };


class Figure
{
public:
	Figure(const fType& type, const COORD& poss);
	~Figure();

	Figure& operator=(const Figure& fig);

	int Rotate(const int& px, const int& py, const int& r) const;

	void RotateFigure();

	void SetBlock();
	
	
	COORD POS{};
	COORD OLD_POS{};

	fType Type{};
	Direction dir = down;
	int rotation_counter = 0;


	std::wstring body;
};

