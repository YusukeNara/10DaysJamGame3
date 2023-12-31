#pragma once
#include "PieceData.h"
#include "PieceDeleteDirection.h"
#include "PieceDeleteTextDrawer.h"
#include "UI.h"
#include "ScoreManager.h"

#include <array>

enum class BoardStatus
{
	WAIT,
	PROCESSING_GENERATE,	//生成
	PROCESSING_ROTATE,		//ピース回転
	PROCESSING_FLOATCHECK,	//浮遊チェック
	PROCESSING_MATCHCHECK,	//ピースマッチチェック
	PRICESSING_SPECIALCHECK,
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
	void UpAndGenerate(bool isNoWait = false);

	void StartPieceRotate(int selectX, int selectY);

	//ピース回転（毎フレーム実行）
	void RotatePiece(int selectX, int selectY);

	void DebugDraw();
	//特殊マッチチェック
	void CheckSpecialMatch();
	//マッチチェック
	void CheckMatch();
	//浮遊チェック
	void CheckFloat();
	//ゲーム時間
	void TimeControl();

	//すべてのマッチ3を有効化
	void ActiveMatch3(bool isEnforce = false);
	//マッチ3有効化
	void EnableMatch3(PIECE_COLOR color);
	//マッチ3を無効化
	void DisableMatch3Active(PIECE_COLOR color);
	//マッチ3のフラグを確認
	bool CheckMatch3Active(PIECE_COLOR color);

	static const int BOARD_WIDTH = 8;
	static const int BOARD_HEIGHT = 11;

	int rotateX = 0, rotateY = 0;

private:
	std::array <std::array<PieceData, BOARD_WIDTH>, BOARD_HEIGHT> boardData;

	BoardStatus boardStatus;
	UI ui;

	//3マッチのアクティブフラグ配列
	std::array<bool, 8> isMatch3Active;

	//現在のレベル
	unsigned int level = 1;
	//フレームカウント
	unsigned int flameCount = 0;
	//生成間隔
	unsigned int generateRemain = 600;
	//生成間隔の減少率
	unsigned int spawnDifficlutyRate = 15;
	//生成ペース下限
	const unsigned int SPAWNTIME_MIN = 300;
	//生成ペース初期値
	const unsigned int SPAWNTIME_MAX = 600;

	//スコア
	unsigned int score = 0;
	//スコアベース
	unsigned int baseScore = 100;
	//スコア倍率（連鎖で上昇）
	float scoreScale = 1.0f;
	//コンボ
	int combo = 0;

	//スペシャルマッチ演出再生用

	//リソース
	int bigFontHandle = 0;
	int smallFontHandle = 0;

	//演出系
	PieceDeleteDirection pieceDeleteDirection;
	PieceDeleteTextDrawer pieceDeleteTextDrawer;

	//マス目のベースを保存

	//マス線の太さ
	const int LINE_WIDTH = 1;
	//マス目描画
	void DrawBoardGrid();

	//ゲーム画面
	int gameSceneGraph = 0;
	//ゲーム画面アルファ値
	Rv3Ease::Rv3Easing alphaEase;

	//スコア
	ScoreManager scoreMgr;


};

