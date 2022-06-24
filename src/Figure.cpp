#include "Figure.h"


Figure::Figure(const fType& type, const COORD& pos) : Type(type), POS(pos), OLD_POS(pos)
{
	SetBlock();
	dir = down;
}

Figure::~Figure()
{

}

void Figure::SetBlock()
{
	body = L"";
	switch (Type)
	{
	case I:
		body.append(tetromino[I]);
		break;
	case T:
		body.append(tetromino[T]);
		break;
	case O:
		body.append(tetromino[O]);
		break;
	case S:
		body.append(tetromino[S]);
		break;
	case Z:
		body.append(tetromino[Z]);
		break;
	case J:
		body.append(tetromino[J]);
		break;
	case L:
		body.append(tetromino[L]);
		break;
	default:
		break;
	}
}

int Figure::Rotate(const int& px, const int& py, const int& r) const
{
	int pi = 0;
	switch (r % 4)
	{
	case 0: // 0 degrees			// 0  1  2  3
		pi = py * 4 + px;			// 4  5  6  7
		break;						// 8  9 10 11
									//12 13 14 15

	case 1: // 90 degrees			//12  8  4  0  
		pi = 12 + py - (px * 4);	//13  9  5  1
		break;						//14 10  6  2
									//15 11  7  3

	case 2: // 180 degrees			//15 14 13 12     
		pi = 15 - (py * 4) - px;	//11 10  9  8
		break;						// 7  6  5  4
									// 3  2  1  0

	case 3: // 270 degrees			// 3  7 11 15
		pi = 3 - py + (px * 4);		// 2  6 10 14
		break;						// 1  5  9 13
	}								// 0  4  8 12

	return pi;
}

void Figure::RotateFigure() 
{
	body.erase();
	int pi{};
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
		{
			pi = tetromino[Type][Rotate(x, y, rotation_counter)];
			body.push_back(pi);
		}
}

Figure & Figure::operator=(const Figure& const fig)
{
	if (&fig == this) return *this;
	POS = fig.POS;
	Type = fig.Type;
	SetBlock();
	return *this;
}

