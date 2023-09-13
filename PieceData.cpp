#include "PieceData.h"

#include "NY_random.h"

#include <DxLib.h>
#include <string>

int PieceData::DRAWBASE_X;
int PieceData::DRAWBASE_Y;

PieceData::PieceData()
{
	DRAWBASE_X = 450 - (PIECE_SIZE * 3) - (PIECE_SIZE / 2);
	DRAWBASE_Y = 450;

	LoadDivGraph("Resources/pieces.png", 8, 8, 1, 32, 32, graphHandles.data());
}

PieceData::~PieceData()
{
}

void PieceData::Init(int x, int y)
{
	posX = x;
	posY = y;
	color = PIECE_COLOR::PCOLOR_NONE;

	int drawCenterX = DRAWBASE_X + posX * PIECE_SIZE;
	int drawCenterY = DRAWBASE_Y - posY * PIECE_SIZE;

	bezierEase.Init(
		RVector3(drawCenterX, drawCenterY, 0),	//始点
		RVector3(drawCenterX, drawCenterY, 0),		//終点
		RVector3(drawCenterX, drawCenterY, 0),
		10, Rv3Ease::RV3_EASE_TYPE::EASE_QUAD_IN);
}

void PieceData::Generate(int x, int y,PIECE_COLOR color)
{
	posX = x;
	posY = y;

	int drawCenterX = DRAWBASE_X + posX * PIECE_SIZE;
	int drawCenterY = DRAWBASE_Y - posY * PIECE_SIZE;
	int drawOldCenterX = DRAWBASE_X + posX * PIECE_SIZE;
	int drawOldCenterY = DRAWBASE_Y - (-1 * PIECE_SIZE);
	int midX = (drawCenterX + drawOldCenterX) / 2;
	int midY = (drawCenterY + drawOldCenterY) / 2;

	bezierEase.Init(
		RVector3(drawOldCenterX, drawOldCenterY, 0),	//始点
		RVector3(drawCenterX, drawCenterY, 0),		//終点
		RVector3(midX, midY, 0),
		10, Rv3Ease::RV3_EASE_TYPE::EASE_QUAD_IN);

	bezierEase.Reset();

	bezierEase.Play();

	int generateColor = 0;

	if (color == PIECE_COLOR::PCOLOR_NONE) {
		generateColor = NY_random::intrand_sl(8, 1);
	}
	else {
		generateColor = int(color);
	}

	this->color = static_cast<PIECE_COLOR>(generateColor);
}

void PieceData::Up()
{
	oldPosX = posX;
	oldPosY = posY;

	posY += 1;

	int drawCenterX = DRAWBASE_X + posX * PIECE_SIZE;
	int drawCenterY = DRAWBASE_Y - posY * PIECE_SIZE;
	int drawOldCenterX = DRAWBASE_X + oldPosX * PIECE_SIZE;
	int drawOldCenterY = DRAWBASE_Y - oldPosY * PIECE_SIZE;
	int midX = (drawCenterX + drawOldCenterX) / 2;
	int midY = (drawCenterY + drawOldCenterY) / 2;

	bezierEase.Init(
		RVector3(drawOldCenterX, drawOldCenterY, 0),	//始点
		RVector3(drawCenterX, drawCenterY, 0),		//終点
		RVector3(midX, midY, 0),
		10, Rv3Ease::RV3_EASE_TYPE::EASE_QUAD_IN);

	bezierEase.Reset();

	bezierEase.Play();
}

void PieceData::Down()
{
}

void PieceData::RotatePiece(int newX, int newY)
{
	//イージング用に旧座標と新座標を保持する
	oldPosX = posX;
	oldPosY = posY;
	posX = newX;
	posY = newY;

	int drawCenterX = DRAWBASE_X + posX * PIECE_SIZE;
	int drawCenterY = DRAWBASE_Y - posY * PIECE_SIZE;
	int drawOldCenterX = DRAWBASE_X + oldPosX * PIECE_SIZE;
	int drawOldCenterY = DRAWBASE_Y - oldPosY * PIECE_SIZE;

	int addX = 0;
	int addY = 0;
	//イージング方向に応じた設定
	if (posX - oldPosX == 1 && posY - oldPosY == 0) {
		addY = PIECE_SIZE / 2;
	}
	else if (posX - oldPosX == 0 && posY - oldPosY == 1) {
		addX = PIECE_SIZE / 2;
	}
	else if (posX - oldPosX == -1 && posY - oldPosY == 0) {
		addY = -PIECE_SIZE / 2;
	}
	else if (posX - oldPosX == 0 && posY - oldPosY == -1) {
		addX = -PIECE_SIZE / 2;
	}

	int midX = (drawCenterX + drawOldCenterX) / 2;
	int midY = (drawCenterY + drawOldCenterY) / 2;

	bezierEase.Reset();

	//イージング座標設定
	bezierEase.Init(
		RVector3(drawOldCenterX, drawOldCenterY, 0),	//始点
		RVector3(drawCenterX, drawCenterY, 0),			//終点
		RVector3(midX + addX, midY + addY, 0),			//イージング制御点
		10, Rv3Ease::RV3_EASE_TYPE::EASE_QUAD_IN);

	bezierEase.Play();
}

void PieceData::SetPos(int newX, int newY)
{
	oldPosX = posX;
	oldPosY = posY;
	posX = newX;
	posY = newY;

	int drawCenterX = DRAWBASE_X + posX * PIECE_SIZE;
	int drawCenterY = DRAWBASE_Y - posY * PIECE_SIZE;
	int drawOldCenterX = DRAWBASE_X + oldPosX * PIECE_SIZE;
	int drawOldCenterY = DRAWBASE_Y - oldPosY * PIECE_SIZE;
	int midX = (drawCenterX + drawOldCenterX) / 2;
	int midY = (drawCenterY + drawOldCenterY) / 2;

	bezierEase.Init(
		RVector3(drawOldCenterX, drawOldCenterY, 0),	//始点
		RVector3(drawCenterX, drawCenterY, 0),		//終点
		RVector3(midX, midY, 0),
		10, Rv3Ease::RV3_EASE_TYPE::EASE_QUAD_IN);

	bezierEase.Reset();

	bezierEase.Play();
}

void PieceData::DeleteReservation()
{
	isDeleteReserved = true;
}

void PieceData::Clear()
{
	color = PIECE_COLOR::PCOLOR_NONE;
	isDeleteReserved = false;
}

void PieceData::Draw(bool isEnable)
{
	bezierEase.Update();

	int easeX = bezierEase.nowPos().x;
	int easeY = bezierEase.nowPos().y;

	int c = 0;
	if (color == PIECE_COLOR::PCOLOR_NONE) { return; }

	switch (color)
	{
	case PIECE_COLOR::PCOLOR_NONE:
		c = GetColor(55, 55, 55);
		break;
	case PIECE_COLOR::PCOLOR_RED:
		useHandle = graphHandles[0];
		break;
	case PIECE_COLOR::PCOLOR_GREEM:
		useHandle = graphHandles[3];
		break;
	case PIECE_COLOR::PCOLOR_BLUE:
		useHandle = graphHandles[1];
		break;
	case PIECE_COLOR::PCOLOR_WHITE:
		useHandle = graphHandles[7];
		break;
	case PIECE_COLOR::PCOLOR_YELLOW:
		useHandle = graphHandles[4];
		break;
	case PIECE_COLOR::PCOLOR_SKY:
		useHandle = graphHandles[2];
		break;
	case PIECE_COLOR::PCOLOR_PURPLE:
		useHandle = graphHandles[5];
		break;
	case PIECE_COLOR::PCOLOR_BLACK:
		useHandle = graphHandles[6];
		break;

	default:
		break;
	}

	int lx = easeX - (PIECE_SIZE / 2) + 4;
	int rx = easeX + (PIECE_SIZE / 2) - 4;
	int ty = easeY - (PIECE_SIZE / 2) + 4;
	int by = easeY + (PIECE_SIZE / 2) - 4;

	if (isEnable) {
		alpha = 255;
	}
	else {
		alpha = 100;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(lx, ty, rx, by, useHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
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

	DrawFormatString(displayX, displayY, GetColor(255, 0, 0), "x:%d y:%d %s", posX, posY, colortxt.c_str());
	DrawFormatString(displayX, displayY + 16, GetColor(255, 0, 0), "easemode %d easeTarget  X:%f Y:%f",
		bezierEase.isEnded(),
		bezierEase.nowPos().x,
		bezierEase.nowPos().y);

}


