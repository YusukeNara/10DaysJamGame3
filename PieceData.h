#pragma once

#include "RVector.h"

enum class PIECE_COLOR
{
	PCOLOR_NONE,
	PCOLOR_RED,
	PCOLOR_GREEM,
	PCOLOR_BLUE,
	PCOLOR_WHITE,
};

class PieceData
{
public:

	PieceData();
	~PieceData();

	void Init(int x, int y);

	//ピースを生成
	void Generate(int x,int y);

	//ピースを上昇
	void Up();

	void Down();

	//ピース座標設定
	void RotatePiece(int newX, int newY);

	void SetPos(int newX, int newY);

	int GetY() { return posY; }

	//ピース消滅
	void Clear();

	//描画
	void Draw();

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

	// ==演算子をオーバーロード
	bool operator==(const PieceData& r) const
	{
		return color == r.color;
	}
	bool operator!=(const PieceData& r) const
	{
		return !(color == r.color);
	}


private:
	int posX = 0;
	int posY = 0;
	int oldPosX = 0;
	int oldPosY = 0;

	//ベジエイージング
	Rv3Ease::Rv3Bezier3 bezierEase;

	PIECE_COLOR color = PIECE_COLOR::PCOLOR_NONE;
};