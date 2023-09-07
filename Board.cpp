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

	boardStatus = BoardStatus::WAIT;

	level = 1;
	flameCount = 0;
	spawnTime = SPAWNTIME_MAX;

	for (int i = 0; i < 3; i++) {
		UpAndGenerate();
	}
}

void Board::Update()
{
	//時間進行
	TimeControl();

	//ステータスに応じて毎フレーム関数を実行
	switch (boardStatus)
	{
	case BoardStatus::WAIT:
		break;
	case BoardStatus::PROCESSING_ROTATE:
		RotatePiece(rotateX, rotateY);
		break;
	case BoardStatus::PROCESSING_FLOATCHECK:
		CheckFloat();
		break;
	case BoardStatus::PROCESSING_MATCHCHECK:
		CheckMatch();
		break;
	case BoardStatus::GAMEOVER:
		break;
	default:
		break;
	}

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
	if (boardStatus == BoardStatus::GAMEOVER) { return; }

	//ピースの座標データを上昇させる
	for (auto& by : boardData)
	{
		for (auto& bx : by) {
			bx.Up();
			if (bx.GetY() == 10 && bx.GetColorNum() != PIECE_COLOR::PCOLOR_NONE) {
				boardStatus = BoardStatus::GAMEOVER;
				break;
			}
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

	//生成時に生成間隔をリセット
	flameCount = 0;
}

void Board::StartPieceRotate(int selectX, int selectY)
{
	if (boardStatus != BoardStatus::WAIT) {
		return;
	}

	rotateX = selectX;
	rotateY = selectY;

	boardStatus = BoardStatus::PROCESSING_ROTATE;
}

void Board::RotatePiece(int selectX, int selectY)
{
	if (boardStatus != BoardStatus::PROCESSING_ROTATE) { return; }

	static int flame = 0;

	//選択中のピース4つを保持（左下、右下、右上、左上）
	if (flame == 0) {
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

	flame++;

	if (flame > 5) {
		boardStatus = BoardStatus::PROCESSING_FLOATCHECK;
		flame = 0;
	}

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

	DrawFormatString(0, 96, GetColor(255, 255, 255), "score : %u", score);
	DrawFormatString(0, 112, GetColor(255, 255, 255), "generate remain : %u", (spawnTime - flameCount) / 60u);
	DrawFormatString(0, 128, GetColor(255, 255, 255), "generate rate : %u", spawnTime / 60u);
	DrawFormatString(0, 144, GetColor(255, 255, 255), "level : %u", level);
	if (boardStatus == BoardStatus::GAMEOVER) {
		DrawFormatString(0, 160, GetColor(255, 255, 255), "GAME OVER");
	}

}

void Board::CheckMatch()
{
	static int flame = 0;

	//消去を行うピースを保持
	std::vector<PieceData*> deletePieceBuff;

	static bool isMatchPiece = false;

	int addScore = 0;

	if (flame == 0) {
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
					isMatchPiece = true;
				}
				if (rightMatchPiece.size() >= 3) {
					for (auto& right : rightMatchPiece) { deletePieceBuff.push_back(right); }
					isMatchPiece = true;
				}
			}
		}

		//ピースを消去開始
		for (auto& d : deletePieceBuff) {
			d->Clear();
			//ピース1つにつき加算スコア増加
			addScore += (baseScore * scoreScale);
		}
		score += addScore;

		//スコアが既定値を超えた場合、レベルアップ
		if (score > level * 10000) {
			level++;
			if (spawnTime > SPAWNTIME_MIN) {
				spawnTime -= spawnDifficlutyRate;
			}
		}
	}

	flame++;

	if (flame > 15) {
		//マッチしているピースがある場合は浮遊チェック
		if (isMatchPiece) { 
			boardStatus = BoardStatus::PROCESSING_FLOATCHECK; 
			//スコア倍率増加
			scoreScale += 0.2f;
		}
		//そうでないなら待機
		else { 
			boardStatus = BoardStatus::WAIT;
			scoreScale = 1.0f;
		}

		flame = 0;
		isMatchPiece = false;
	}



}

void Board::CheckFloat()
{
	static int flame = 0;

	if (flame == 0) {
		for (int y = 1; y < BOARD_HEIGHT; y++) {
			for (int x = 0; x < BOARD_WIDTH; x++) {
				if (boardData[y][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) { continue; }

				int newY = y;
				bool downFlag = false;
				//自ピースの下にピースが存在するか、一番下につくまで下がり続ける
				while (boardData[newY - 1][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) {
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

	flame++;
	if (flame > 5) {
		boardStatus = BoardStatus::PROCESSING_MATCHCHECK;
		flame = 0;
	}

}

void Board::TimeControl()
{
	//マッチチェック中は時間進行しない
	//本来はコンボ進行中のみ進行しない
	if (boardStatus == BoardStatus::PROCESSING_MATCHCHECK) {
		return;
	}

	flameCount++;

	if (flameCount > spawnTime) {
		UpAndGenerate();
	}

}
