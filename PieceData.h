#pragma once

#include "RVector.h"

#include <array>

enum class PIECE_COLOR
{
	PCOLOR_NONE,
	PCOLOR_RED,
	PCOLOR_GREEM,
	PCOLOR_BLUE,
	PCOLOR_WHITE,
	PCOLOR_YELLOW,
	PCOLOR_SKY,
	PCOLOR_PURPLE,
	PCOLOR_BLACK,
};

class PieceData
{
public:

	PieceData();
	~PieceData();

	void Init(int x, int y);

	//ピースを生成
	void Generate(int x, int y, PIECE_COLOR color = PIECE_COLOR::PCOLOR_NONE);

	//ピースを上昇
	void Up();

	void Down();

	//ピース座標設定
	void RotatePiece(int newX, int newY);

	void SetPos(int newX, int newY);

	int GetX() { return posX; }
	int GetY() { return posY; }

	void DeleteReservation();
	//ピース消滅
	void Clear();

	//描画
	void Draw(bool isEnable);

	void DisplayPieceInfo(int displayX, int displayY);

	PIECE_COLOR GetColorNum() { return color; }

	//描画左端基準
	static int DRAWBASE_X;
	//描画下基準
	static int DRAWBASE_Y;
	//ピース描画サイズ
	static const int PIECE_SIZE = 40;
	//イージングフレーム数
	static const int EASE_MOVE_TIME = 10;

	// ==演算子をオーバーロード（同色のピースを等価演算子で判断する）
	bool operator==(const PieceData& r) const
	{
		return color == r.color;
	}
	// !=演算子をオーバーロード（同色のピースを等価演算子で判断する）
	bool operator!=(const PieceData& r) const
	{
		return !(color == r.color);
	}

	bool GetDeleteReserved() { return isDeleteReserved; }

	int GetDrawCenterX() { return DRAWBASE_X + posX * PIECE_SIZE; };
	int GetDrawCenterY() { return DRAWBASE_Y - posY * PIECE_SIZE; };

private:
	int posX = 0;
	int posY = 0;
	int oldPosX = 0;
	int oldPosY = 0;
	//消去される予定のピース（ピースを消すときの判定に使用しないフラグ）
	int isDeleteReserved = false;

	//ベジエイージング
	Rv3Ease::Rv3Bezier3 bezierEase;

	//画像ハンドル
	std::array<int, 8> graphHandles;
	int useHandle = 0;
	std::array<int, 8> disableHandle;

	int alpha = 255;

	PIECE_COLOR color = PIECE_COLOR::PCOLOR_NONE;
};