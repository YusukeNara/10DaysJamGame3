#pragma once


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

	//ピース座標設定
	void SetPos(int newX, int newY);

	//ピース消滅
	void Clear();

	//描画
	void Draw();

	void DisplayPieceInfo(int displayX, int displayY);

	static const int DRAWBASE_X = 354;
	static const int DRAWBASE_Y = 500;

private:
	int posX = 0;
	int posY = 0;

	PIECE_COLOR color = PIECE_COLOR::PCOLOR_NONE;
};