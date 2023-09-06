#pragma once
#include "PieceData.h"

#include <array>

enum class BoardStatus
{
	WAIT,
	PROCESSING_ROTATE,		//ピース回転
	PROCESSING_FLOATCHECK,	//浮遊チェック
	PROCESSING_MATCHCHECK,	//ピースマッチチェック
	GAMEOVER,				//ゲームオーバー
};

//ボードの状態を管理するクラス
class Board
{
public:
	Board();
	~Board();

	void Init();

	void Update();

	void Draw();

	//ピース上昇
	void UpAndGenerate();

	void StartPieceRotate(int selectX, int selectY);

	//ピース回転（毎フレーム実行）
	void RotatePiece(int selectX, int selectY);

	void DebugDraw();

	void CheckMatch();

	void CheckFloat();

	void TimerControl();


	static const int BOARD_WIDTH = 6;
	static const int BOARD_HEIGHT = 11;

	int rotateX = 0, rotateY = 0;

private:
	std::array <std::array<PieceData, 6>, 11> boardData;

	BoardStatus boardStatus;

	unsigned int level = 1;

	unsigned int timer = 3000;
	//リセットされる時間
	unsigned int resetTime = 3000;
	//リセットされる時間の減少幅
	unsigned int mintime = 120;
	//リセットされる時間の下限
	const unsigned int RESETTIME_MIN = 360;

	//スコア
	unsigned int score = 0;
	//加算するスコア
	unsigned int addScore = 100;
	//連鎖倍率
	float comboScale = 1.0f;
	//レベルアップに必要なスコア
	unsigned int levelupScore = 10000;
	//レベルアップに必要なスコアの上昇幅
	unsigned int levelupScoreBase = 10000;

};

