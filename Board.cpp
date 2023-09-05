#include "Board.h"

#include <vector>
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

	//揃っているピースがあるかを確認する
	CheckMatch();


	//浮いているピースを検知して処理
	CheckFloat();

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
			int psize = PieceData::PIECE_SIZE;
			if (mouseX > PieceData::DRAWBASE_X + x * psize -16 && mouseX < PieceData::DRAWBASE_X + x * psize + psize - (psize / 2)) {
				if (mouseY > PieceData::DRAWBASE_Y - y * psize - 16 && mouseY < PieceData::DRAWBASE_Y - y * psize + psize - (psize / 2)) {
					bx.DisplayPieceInfo(mouseX, mouseY);
				}
			}
			x++;
		}
		y++;
	}

	DrawFormatString(0, 0, GetColor(255, 255, 255), "x %d  y %d", mouseX, mouseY);
	DrawFormatString(0, 16, GetColor(255, 255, 255), "WASD :   box move");
	DrawFormatString(0, 32, GetColor(255, 255, 255), "space :  piece rotate (反時計回り)");
	DrawFormatString(0, 48, GetColor(255, 255, 255), "U :  piece generate");
	DrawFormatString(0, 64, GetColor(255, 255, 255), "R :  reset");
}

void Board::CheckMatch()
{
	//消去を行うピースを保持
	std::vector<PieceData*> deletePieceBuff;

	//左下からチェック
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (boardData[y][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) {
				continue;
			}
			//上のピース情報保持
			std::vector<PieceData*> upperMatchPiece;
			//右のピース情報保持
			std::vector<PieceData*> rightMatchPiece;
			//右で異なる色を見つけるまでループ
			for (int xx = x; xx < BOARD_WIDTH; xx++) {
				if (boardData[y][x] != boardData[y][xx]) { break; }
				rightMatchPiece.push_back(&boardData[y][xx]);
			}
			//上で異なる色を見つけるまでループ
			for (int yy = y; yy < BOARD_HEIGHT; yy++) {
				if (boardData[y][x] != boardData[yy][x]) { break; }
				upperMatchPiece.push_back(&boardData[yy][x]);
			}

			//それぞれ3つ以上ある場合、消去コンテナに保持
			if (upperMatchPiece.size() >= 3) {
				for (auto& up : upperMatchPiece) { deletePieceBuff.push_back(up); }
			}
			if (rightMatchPiece.size() >= 3) {
				for (auto& right : rightMatchPiece) { deletePieceBuff.push_back(right); }
			}
		}
	}

	//ピースを消去開始
	for (auto& d : deletePieceBuff) {
		d->Clear();
	}

}

void Board::CheckFloat()
{
	for (int y = 1; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (boardData[y][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) { continue; }

			int newY = y;
			bool downFlag = false;
			//自ピースの下にピースが存在するか、一番下につくまで下がり続ける
			while (boardData[newY - 1][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE){
				downFlag = true;
				newY--;
				if (newY == 0) { break; }
			}
			
			//新y座標の位置のピース情報に代入し、旧y座標のピースをnoneに
			if (downFlag) {
				boardData[newY][x] = boardData[y][x];
				boardData[newY][x].SetPos(x, newY);
				boardData[y][x].Clear();
			}

		}
	}





}
