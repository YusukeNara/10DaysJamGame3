#include "Board.h"

#include <algorithm>
#include <DxLib.h>

Board::Board()
{
}

Board::~Board()
{
}

void Board::Init()
{
	int y = 0;
	for (auto& by : boardData)
	{
		int x = 0;
		for (auto& bx : by) {
			bx.Init(x, y);
			x++;
		}
		y++;
	}
}

void Board::Update()
{

	//浮いているピースを検知して処理


	//揃っているピースがあるかを確認する



}

void Board::Draw()
{
	int y = 0;
	for (auto& by : boardData)
	{
		for (auto& bx : by) {
			bx.Draw();
		}
		y++;
		if (y == 10) {
			break;
		}
	}
}

void Board::UpAndGenerate()
{
	//ピースの座標データを上昇させる
	for (auto& by : boardData)
	{
		for (auto& bx : by) {
			bx.Up();
		}
	}

	//このとき天井（非表示の11列目）を超えるピースが存在する場合ゲームオーバーフラグ発生


	//ピースデータを右シフトする（上昇と同義）
	std::shift_right(boardData.begin(), boardData.end(), 1);

	//最下段に新たな要素を代入する
	std::array<PieceData, 6> initdata;
	boardData[0] = initdata;
	
	int x = 0;
	for (auto& b : boardData[0]) {
		b.Generate(x, 0);
		x++;
	}

}

void Board::RotatePiece(int selectX, int selectY)
{
	//選択中のピース4つを保持（左下、右下、右上、左上）
	std::array<PieceData, 4> rotateP = {
		boardData[selectY][selectX],
		boardData[selectY][selectX + 1],
		boardData[selectY + 1][selectX + 1],
		boardData[selectY + 1][selectX] };

	//左下->右下
	rotateP[0].SetPos(selectX + 1, selectY);
	boardData[selectY][selectX + 1] = rotateP[0];
	//右下->右上
	rotateP[1].SetPos(selectX + 1, selectY + 1);
	boardData[selectY + 1][selectX + 1] = rotateP[1];
	//右上->左上
	rotateP[2].SetPos(selectX, selectY + 1);
	boardData[selectY + 1][selectX] = rotateP[2];
	//左上->左下
	rotateP[3].SetPos(selectX, selectY);
	boardData[selectY][selectX] = rotateP[3];

}

void Board::DebugDraw()
{

	int mouseX = 0, mouseY = 0;

	GetMousePoint(&mouseX, &mouseY);

	int y = 0;
	for (auto& by : boardData)
	{
		int x = 0;
		for (auto& bx : by) {

			if (mouseX > PieceData::DRAWBASE_X + x * 32 -16 && mouseX < PieceData::DRAWBASE_X + x * 32 + 32 - 16) {
				if (mouseY > PieceData::DRAWBASE_Y - y * 32 - 16 && mouseY < PieceData::DRAWBASE_Y - y * 32 + 32 - 16) {
					bx.DisplayPieceInfo(mouseX, mouseY);
				}
			}
			x++;
		}
		y++;
	}

	DrawFormatString(0, 0, GetColor(100, 100, 100), "x %d  y %d", mouseX, mouseY);
}

void Board::CheckMatch()
{
	//揃っているピースがないか確認



}
