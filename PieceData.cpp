#include "PieceData.h"

#include "NY_random.h"

#include <DxLib.h>
#include <string>

PieceData::PieceData()
{
}

PieceData::~PieceData()
{
}

void PieceData::Init(int x, int y)
{
	posX = x;
	posY = y;
	color = PIECE_COLOR::PCOLOR_NONE;
}

void PieceData::Generate(int x, int y)
{
	posX = x;
	posY = y;

	int generateColor = NY_random::intrand_sl(4, 1);

	color = static_cast<PIECE_COLOR>(generateColor);
}

void PieceData::Up()
{
	posY += 1;
}

void PieceData::SetPos(int newX, int newY)
{
	posX = newX;
	posY = newY;
}

void PieceData::Clear()
{
	color = PIECE_COLOR::PCOLOR_NONE;
}

void PieceData::Draw()
{
	int c = 0;
	//if (color == PIECE_COLOR::PCOLOR_NONE) { return; }

	switch (color)
	{
	case PIECE_COLOR::PCOLOR_NONE:
		c = GetColor(55, 55, 55);
		break;
	case PIECE_COLOR::PCOLOR_RED:
		c = GetColor(255, 0, 0);
		break;
	case PIECE_COLOR::PCOLOR_GREEM:
		c = GetColor(0, 255, 0);
		break;
	case PIECE_COLOR::PCOLOR_BLUE:
		c = GetColor(0, 0, 255);
		break;
	case PIECE_COLOR::PCOLOR_WHITE:
		c = GetColor(255, 255, 255);
		break;
	default:
		break;
	}

	DrawCircle(DRAWBASE_X + posX * 32, DRAWBASE_Y - posY * 32, 16, c);
}

void PieceData::DisplayPieceInfo(int displayX, int displayY)
{
	std::string colortxt;
	switch (color)
	{
	case PIECE_COLOR::PCOLOR_NONE:
		colortxt = "none";
		break;
	case PIECE_COLOR::PCOLOR_RED:
		colortxt = "red";
		break;
	case PIECE_COLOR::PCOLOR_GREEM:
		colortxt = "green";
		break;
	case PIECE_COLOR::PCOLOR_BLUE:
		colortxt = "blue";
		break;
	case PIECE_COLOR::PCOLOR_WHITE:
		colortxt = "white";
		break;
	default:
		break;
	}

	DrawFormatString(displayX, displayY, GetColor(100, 100, 100), "x:%d y:%d %s", posX, posY, colortxt.c_str());


}

