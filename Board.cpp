#include "Board.h"

#include "Input.h"
#include <vector>
#include <algorithm>
#include <DxLib.h>

#include "NY_random.h"

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
	ui.Init();
	level = 1;
	flameCount = 0;
	generateRemain = SPAWNTIME_MAX;

	for (int i = 0; i < 5; i++) {
		UpAndGenerate(true);
	}

	for (auto& m : isMatch3Active) {
		m = true;
	}

	//リソース読み込み
	bigFontHandle = CreateFontToHandle("03スマートフォントUI", 64, 9, DX_FONTTYPE_NORMAL);
	smallFontHandle = CreateFontToHandle("03スマートフォントUI", 16, 6, DX_FONTTYPE_NORMAL);
	//fontHandle = LoadFontDataToHandle("Resources/SmartFontUI.dft", 0);

	pieceDeleteDirection.Init();
	pieceDeleteTextDrawer.Init();

	scoreMgr.Init();

	alphaEase.Init(Rv3Ease::RV3_EASE_TYPE::EASE_CUBIC_IN, RVector3(255, 0, 0), RVector3(100, 0, 0), 60);
	alphaEase.Reset();
	
	score = 0;
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
	case BoardStatus::PROCESSING_GENERATE:
		UpAndGenerate();
		break;
	case BoardStatus::PROCESSING_ROTATE:
		RotatePiece(rotateX, rotateY);
		break;
	case BoardStatus::PROCESSING_FLOATCHECK:
		CheckFloat();
		break;
	case BoardStatus::PRICESSING_SPECIALCHECK:
		CheckSpecialMatch();
		break;
	case BoardStatus::PROCESSING_MATCHCHECK:
		CheckMatch();
		break;
	case BoardStatus::GAMEOVER:
		alphaEase.Play();
		scoreMgr.StartScoreDraw(score, level);
		break;
	default:
		break;
	}

	//すべてのマッチ3が無効化されたら再度有効化
	ActiveMatch3();

	ui.Update();

	//演出系更新
	alphaEase.Update();
	pieceDeleteDirection.Update();
	pieceDeleteTextDrawer.Update();
	
	scoreMgr.Update();

#ifdef  _DEBUG
	
	if (Input::IsKeyTrigger(KEY_INPUT_L)) { level++; }

#endif //  _DEBUG


}

void Board::Draw()
{
	gameSceneGraph = MakeScreen(900, 506, TRUE);
	SetDrawScreen(gameSceneGraph);

	int y = 0;
	for (auto& by : boardData)
	{
		for (auto& bx : by) {
			bx.Draw(CheckMatch3Active(bx.GetColorNum()));
		}
		y++;
		if (y == 10) {
			break;
		}
	}

	ui.DrawFlame();
	ui.DrawLevel(level);
	ui.DrawScore(score);
	ui.DrawTime((generateRemain - flameCount) / 60u);
	DrawBoardGrid();

	pieceDeleteDirection.Draw();
	pieceDeleteTextDrawer.Draw();

	SetDrawScreen(DX_SCREEN_BACK);

	if (boardStatus == BoardStatus::GAMEOVER) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaEase.GetNowpos().x);
	}
	else {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	DrawGraph(0, 0, gameSceneGraph, TRUE);
	DeleteGraph(gameSceneGraph);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	scoreMgr.Draw();

}

void Board::UpAndGenerate(bool isNoWait)
{
	if (boardStatus == BoardStatus::GAMEOVER) { return; }

	static int frame = 0;

	if (frame == 0) {
		//ピースの座標データを上昇させる
		for (auto& by : boardData)
		{
			for (auto& bx : by) {
				bx.Up();
				if (bx.GetY() == BOARD_HEIGHT - 1 && bx.GetColorNum() != PIECE_COLOR::PCOLOR_NONE) {


					boardStatus = BoardStatus::GAMEOVER;
					break;
				}
			}
		}

		//このとき天井（非表示の11列目）を超えるピースが存在する場合ゲームオーバーフラグ発生

		//ピースデータを右シフトする（上昇と同義）
		std::shift_right(boardData.begin(), boardData.end(), 1);

		//最下段に新たな要素を代入する
		std::array<PieceData, BOARD_WIDTH> initdata;
		boardData[0] = initdata;

		//ボード上のピースの数
		std::array<int, 8> pcolorCount{ 0,0,0,0,0,0,0,0 };

		for (auto& y : boardData) {
			//PCOLOR_NUMの順番に配列に保存
			pcolorCount[0] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_RED; });
			pcolorCount[1] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_GREEM; });
			pcolorCount[2] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_BLUE; });
			pcolorCount[3] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_WHITE; });
			pcolorCount[4] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_YELLOW; });
			pcolorCount[5] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_SKY; });
			pcolorCount[6] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_PURPLE; });
			pcolorCount[7] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_BLACK; });
		}

		//一番少ないピース番号を保持

		std::array<int, 8>::iterator itr;
		if (level < 5) {
			itr = std::min_element(pcolorCount.begin(), pcolorCount.end() - 4);
		}
		else if (level < 10) {
			itr = std::min_element(pcolorCount.begin(), pcolorCount.end() - 1);
		}
		else {
			itr = std::min_element(pcolorCount.begin(), pcolorCount.end());
		}
		int minPiece = int(std::distance(pcolorCount.begin(), itr));


		int x = 0;
		for (auto& b : boardData[0]) {
			//レベルに応じて生成するピースが異なる
			if (level < 5) {
				int colorNum = NY_random::intrand_sl(4, 1);
				const int MIN_PIECE_GENERATE_RATE = 75;
				if (NY_random::intrand_sl(100, 0) < MIN_PIECE_GENERATE_RATE) {
					//ボード内で最も少ないピースを生成
					colorNum = minPiece + 1;
					//ただし同色ルールは適用
					//上左で同色を生成しない
					if (x == 0) {
						//上2マス両方が同色のときは再度生成
						while (PIECE_COLOR(colorNum) == boardData[1][x].GetColorNum() && PIECE_COLOR(colorNum) == boardData[2][x].GetColorNum()) {
							colorNum = NY_random::intrand_sl(4, 1);
						}
					}
					else {
						while (PIECE_COLOR(colorNum) == boardData[0][x - 1].GetColorNum()) {
							colorNum = NY_random::intrand_sl(4, 1);
						}
					}
				}
				else {
					if (x == 0) {
						//上2マス両方が同色のときは再度生成
						while (PIECE_COLOR(colorNum) == boardData[1][x].GetColorNum() && PIECE_COLOR(colorNum) == boardData[2][x].GetColorNum()) {
							colorNum = NY_random::intrand_sl(4, 1);
						}
					}
					else {
						while (PIECE_COLOR(colorNum) == boardData[0][x - 1].GetColorNum()) {
							colorNum = NY_random::intrand_sl(4, 1);
						}
					}
				}
				b.Generate(x, 0, PIECE_COLOR(colorNum));
			}
			// ------------------------------------------------------------------------------------------ //
			else if (level < 10) {
				int colorNum = 0;
				//最小ピース生成確率
				const int MIN_PIECE_GENERATE_RATE = 75;
				if (NY_random::intrand_sl(100, 0) < MIN_PIECE_GENERATE_RATE) {
					//ボード内で最も少ないピースを生成
					colorNum = minPiece + 1;
					//ただし同色ルールは適用
					//上左で同色を生成しない
					if (x == 0) {
						//上2マス両方が同色のときは再度生成
						while (PIECE_COLOR(colorNum) == boardData[1][x].GetColorNum() && PIECE_COLOR(colorNum) == boardData[2][x].GetColorNum()) {
							colorNum = NY_random::intrand_sl(7, 1);
						}
					}
					else {
						while (PIECE_COLOR(colorNum) == boardData[0][x - 1].GetColorNum()) {
							colorNum = NY_random::intrand_sl(7, 1);
						}
					}
				}
				else {
					colorNum = NY_random::intrand_sl(7, 1);
					//上左で同色を生成しない
					if (x == 0) {
						//上2マス両方が同色のときは再度生成
						while (PIECE_COLOR(colorNum) == boardData[1][x].GetColorNum() && PIECE_COLOR(colorNum) == boardData[2][x].GetColorNum()) {
							colorNum = NY_random::intrand_sl(7, 1);
						}
					}
					else {
						while (PIECE_COLOR(colorNum) == boardData[0][x - 1].GetColorNum()) {
							colorNum = NY_random::intrand_sl(7, 1);
						}
					}
				}

				b.Generate(x, 0, PIECE_COLOR(colorNum));
			}
			// ------------------------------------------------------------------------------------------ //
			else {
				//ピースの総数を計算し、最も少ないピースの生成率を上げる
				int colorNum = 0;
				//最小ピース生成確率
				const int MIN_PIECE_GENERATE_RATE = 75;
				if (NY_random::intrand_sl(100, 0) < MIN_PIECE_GENERATE_RATE) {
					//ボード内で最も少ないピースを生成
					colorNum = minPiece + 1;
					//ただし同色ルールは適用
					//上左で同色を生成しない
					if (x == 0) {
						//上2マス両方が同色のときは再度生成
						while (PIECE_COLOR(colorNum) == boardData[1][x].GetColorNum() && PIECE_COLOR(colorNum) == boardData[2][x].GetColorNum()) {
							colorNum = NY_random::intrand_sl(8, 1);
						}
					}
					else {
						while (PIECE_COLOR(colorNum) == boardData[0][x - 1].GetColorNum()) {
							colorNum = NY_random::intrand_sl(8, 1);
						}
					}
				}
				else {
					colorNum = NY_random::intrand_sl(8, 1);
					//上左で同色を生成しない
					if (x == 0) {
						//上2マス両方が同色のときは再度生成
						while (PIECE_COLOR(colorNum) == boardData[1][x].GetColorNum() && PIECE_COLOR(colorNum) == boardData[2][x].GetColorNum()) {
							colorNum = NY_random::intrand_sl(8, 1);
						}
					}
					else {
						while (PIECE_COLOR(colorNum) == boardData[0][x - 1].GetColorNum()) {
							colorNum = NY_random::intrand_sl(8, 1);
						}
					}
				}
				b.Generate(x, 0,PIECE_COLOR(colorNum));
			}

			x++;
		}
	}

	if (boardStatus == BoardStatus::GAMEOVER) { return; }

	if (isNoWait) {
		frame = 0;
		boardStatus = BoardStatus::WAIT;
	}
	else {
		frame++;
		if (frame == 10) {
			flameCount = 0;
			frame = 0;
			boardStatus = BoardStatus::WAIT;
		}
	}

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
		rotateP[0].RotatePiece(selectX + 1, selectY);
		boardData[selectY][selectX + 1] = rotateP[0];
		//右下->右上
		rotateP[1].RotatePiece(selectX + 1, selectY + 1);
		boardData[selectY + 1][selectX + 1] = rotateP[1];
		//右上->左上
		rotateP[2].RotatePiece(selectX, selectY + 1);
		boardData[selectY + 1][selectX] = rotateP[2];
		//左上->左下
		rotateP[3].RotatePiece(selectX, selectY);
		boardData[selectY][selectX] = rotateP[3];
	}

	flame++;

	if (flame > 10) {
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

}

void Board::CheckSpecialMatch()
{

	static int flame = 0;
	//処理フレーム
	const int EXECUTION_FRAME = 60;

	//ストレートマッチフラグ
	bool isMatchStraight = false;
	//ダイヤモンドマッチフラグ
	bool isMatchDiamond = false;
	//ビクトリーマッチフラグ
	bool isMatchVictory = false;
	//マッチを確認
	static bool isMatchPiece = false;

	int addScore = 0;

	//消去するピースまとめ
	std::vector<PieceData*> deletePieceBuff;

	if (flame == 0) {
		//ストレートチェック
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
				//右で異なる色を見つけるor消去済検知までループ
				for (int xx = x; xx < BOARD_WIDTH; xx++) {
					if (boardData[y][x] != boardData[y][xx]
						|| boardData[y][xx].GetDeleteReserved()) {
						break;
					}
					rightMatchPiece.push_back(&boardData[y][xx]);
				}
				//上で異なる色を見つけるor消去済検知までループ
				for (int yy = y; yy < BOARD_HEIGHT; yy++) {
					if (boardData[y][x] != boardData[yy][x]
						|| boardData[yy][x].GetDeleteReserved()) {
						break;
					}
					upperMatchPiece.push_back(&boardData[yy][x]);
				}

				//それぞれ5つ以上ある場合、揃ったピースと同色ピースを消去コンテナに保持
				if (upperMatchPiece.size() >= 5) {
					int count = 0;
					for (auto& up : upperMatchPiece) {
						up->DeleteReservation();
						deletePieceBuff.push_back(up);
						count++;
					}
					//同色ピースも消去
					for (auto& y : boardData) {
						for (auto& x : y) {
							if (x == *deletePieceBuff[0] && !x.GetDeleteReserved()) {
								deletePieceBuff.push_back(&x);
							}
						}
					}
					isMatchPiece = true;
					isMatchStraight = true;
				}
				if (rightMatchPiece.size() >= 5) {
					int count = 0;
					for (auto& right : rightMatchPiece) {
						right->DeleteReservation();
						deletePieceBuff.push_back(right);
						count++;
					}
					//同色ピースも消去
					for (auto& y : boardData) {
						for (auto& x : y) {
							if (x == *deletePieceBuff[0] && !x.GetDeleteReserved()) {
								deletePieceBuff.push_back(&x);
							}
						}
					}
					isMatchPiece = true;
					isMatchStraight = true;
				}
				if (isMatchStraight) { break; }
			}
			if (isMatchStraight) { break; }
		}

		//マッチしたらスキップする
		if (!isMatchStraight) {
			//ダイヤモンドチェック
			for (int y = 0; y < BOARD_HEIGHT - 4; y++) {
				//ダイヤモンドは端から検知しない
				for (int x = 1; x < BOARD_WIDTH - 1; x++) {	//
					if (boardData[y][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) {
						continue;
					}
					//消すピースのまとまりを保持
					std::vector<PieceData*> matchPiece;
					if (boardData[y][x] == boardData[y + 3][x] &&
						boardData[y][x] == boardData[y + 2][x - 1] &&
						boardData[y][x] == boardData[y + 1][x - 1] &&
						boardData[y][x] == boardData[y + 2][x + 1] &&
						boardData[y][x] == boardData[y + 1][x + 1]) {
						//該当する場所を削除バッファに保存
						matchPiece.push_back(&boardData[y][x]);
						matchPiece.push_back(&boardData[y + 3][x]);
						matchPiece.push_back(&boardData[y + 2][x - 1]);
						matchPiece.push_back(&boardData[y + 2][x + 1]);
						matchPiece.push_back(&boardData[y + 1][x - 1]);
						matchPiece.push_back(&boardData[y + 1][x + 1]);
						isMatchDiamond = true;
					}
					if (isMatchDiamond) {
						isMatchPiece = true;
						for (auto& group : matchPiece) {
							group->DeleteReservation();
							deletePieceBuff.push_back(group);
						}
						//すべての色を消去可能に
						ActiveMatch3(true);
						
						break;
					}
				}
				if (isMatchDiamond) { break; }
			}
		}

		if (!isMatchDiamond) {
			//ビクトリーチェック
			for (int y = 0; y < BOARD_HEIGHT - 3; y++) {
				for (int x = 2; x < BOARD_WIDTH - 2; x++) {
					if (boardData[y][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) {
						continue;
					}
					//消すピースのまとまりを保持
					std::vector<PieceData*> matchPiece;
					if (boardData[y][x] == boardData[y + 2][x - 2] &&
						boardData[y][x] == boardData[y + 1][x - 1] &&
						boardData[y][x] == boardData[y + 2][x + 2] &&
						boardData[y][x] == boardData[y + 1][x + 1]) {
						//該当する場所を削除バッファに保存
						matchPiece.push_back(&boardData[y][x]);
						matchPiece.push_back(&boardData[y + 2][x - 2]);
						matchPiece.push_back(&boardData[y + 2][x + 2]);
						matchPiece.push_back(&boardData[y + 1][x - 1]);
						matchPiece.push_back(&boardData[y + 1][x + 1]);
						isMatchVictory = true;
					}
					if (isMatchVictory) {
						isMatchPiece = true;
						for (auto& group : matchPiece) {
							group->DeleteReservation();
							deletePieceBuff.push_back(group);
						}
						//すべての色を消去可能に
						ActiveMatch3(true);
						break;
					}

				}
				if (isMatchVictory) { break; }
			}
		}
	}

	//ピースを消去開始
	for (auto& d : deletePieceBuff) {
		//演出再生位置
		int ease_posX = PieceData::DRAWBASE_X +
			d->GetX() * PieceData::PIECE_SIZE +
			PieceData::PIECE_SIZE / 2;
		//演出再生位置
		int ease_posY = PieceData::DRAWBASE_Y -
			d->GetY() * PieceData::PIECE_SIZE -
			PieceData::PIECE_SIZE / 2;
		//演出再生
		pieceDeleteDirection.PlayEase(ease_posX, ease_posY, 0, 0, EXECUTION_FRAME,5);
		d->Clear();
		//ピース1つにつき加算スコア増加
		addScore += (baseScore * scoreScale);
	}

	//該当するスペシャルマッチに応じたスコア加算、演出再生、特殊効果発生
	if (isMatchStraight) {
		addScore += 5000;
		//ストレート演出再生
		pieceDeleteTextDrawer.DrawSpecialText(0);
		combo++;
		score += addScore * scoreScale;
		ui.AddScore(addScore);
	}
	if (isMatchDiamond) {
		addScore += 5000;
		//ストレート演出再生
		pieceDeleteTextDrawer.DrawSpecialText(1);
		combo++;
		score += addScore * scoreScale;
		ui.AddScore(addScore);
	}
	if (isMatchVictory) {
		addScore += 5000;
		//ストレート演出再生
		pieceDeleteTextDrawer.DrawSpecialText(2);
		combo++;
		score += addScore * scoreScale;
		ui.AddScore(addScore);
	}

	
	flame++;
	//マッチしてるピースがない
	if (!isMatchPiece) {
		boardStatus = BoardStatus::PROCESSING_MATCHCHECK;
		scoreScale = 1.0f;
		flame = 0;
		isMatchPiece = false;
	}
	//マッチしている
	else if(flame >= EXECUTION_FRAME){
		boardStatus = BoardStatus::PROCESSING_FLOATCHECK;
		//スコア倍率増加
		scoreScale += 0.5f;

		flame = 0;
		isMatchPiece = false;
	}

}

void Board::CheckMatch()
{
	static int flame = 0;
	//処理フレーム
	const int EXECUTION_FRAME = 30;

	//消去を行うピースを保持
	std::vector<PieceData*> deletePieceBuff;
	//消去するピースのまとまりを保持（コンボ用）
	std::vector<std::vector<PieceData*>> deletePieceGroupBuff;

	static bool isMatchPiece = false;

	int addScore = 0;

	if (flame == 0) {
		//左下からチェック
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			for (int x = 0; x < BOARD_WIDTH; x++) {
				if (boardData[y][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) {
					continue;
				}
				if (!CheckMatch3Active(boardData[y][x].GetColorNum())) {
					continue;
				}
				//上のピース情報保持
				std::vector<PieceData*> upperMatchPiece;
				//右のピース情報保持
				std::vector<PieceData*> rightMatchPiece;
				//右で異なる色を見つけるor消去済検知までループ
				for (int xx = x; xx < BOARD_WIDTH; xx++) {
					if (boardData[y][x] != boardData[y][xx]
						|| boardData[y][xx].GetDeleteReserved()) {
						break;
					}
					rightMatchPiece.push_back(&boardData[y][xx]);
				}
				//上で異なる色を見つけるor消去済検知までループ
				for (int yy = y; yy < BOARD_HEIGHT; yy++) {
					if (boardData[y][x] != boardData[yy][x]
						|| boardData[yy][x].GetDeleteReserved()) {
						break;
					}
					upperMatchPiece.push_back(&boardData[yy][x]);
				}

				//それぞれ3つ以上ある場合、消去コンテナに保持
				if (upperMatchPiece.size() >= 3) {
					//コンボ加算
					combo++;
					for (auto& up : upperMatchPiece) { 
						deletePieceBuff.push_back(up); 
						up->DeleteReservation();
					}
					//コンボ表示
					pieceDeleteTextDrawer.DrawComboText((*upperMatchPiece.begin())->GetDrawCenterX(),
						(*upperMatchPiece.begin())->GetDrawCenterY(),
						EXECUTION_FRAME,
						combo);

					isMatchPiece = true;
				}
				if (rightMatchPiece.size() >= 3) {
					//コンボ加算
					combo++;
					for (auto& right : rightMatchPiece) { 
						right->DeleteReservation();
						deletePieceBuff.push_back(right); 
					}
					//コンボ表示
					pieceDeleteTextDrawer.DrawComboText((*rightMatchPiece.begin())->GetDrawCenterX(),
						(*rightMatchPiece.begin())->GetDrawCenterY(),
						EXECUTION_FRAME,
						combo);
					isMatchPiece = true;
				}
			}
		}

		//ピースを消去開始
		for (auto& d : deletePieceBuff) {
			//演出再生位置
			int ease_posX = PieceData::DRAWBASE_X + 
				d->GetX() * PieceData::PIECE_SIZE + 
				PieceData::PIECE_SIZE / 2;
			//演出再生位置
			int ease_posY = PieceData::DRAWBASE_Y -
				d->GetY() * PieceData::PIECE_SIZE -
				PieceData::PIECE_SIZE / 2;
			//演出再生
			pieceDeleteDirection.PlayEase(ease_posX, ease_posY, 0, 0, EXECUTION_FRAME);

			//消した色の消去可能フラグを非アクティブ化
			DisableMatch3Active(d->GetColorNum());

			//ピース消去
			d->Clear();

			//ピース1つにつき加算スコア増加
			addScore += (baseScore * scoreScale);
		}
		int oldScore;
		oldScore = score;
		score += addScore * scoreScale;
		if (oldScore != score) {
			ui.AddScore(addScore);
		}

		//スコアが既定値を超えた場合、レベルアップ
		if (score > level * 5000) {
			level++;
			ui.LevelUp();
			if (generateRemain > SPAWNTIME_MIN) {
				generateRemain -= spawnDifficlutyRate;
			}
		}
	}

	flame++;
	//マッチしてるピースがない
	if (!isMatchPiece) {
		boardStatus = BoardStatus::WAIT;
		scoreScale = 1.0f;
		combo = 0;
		flame = 0;
		isMatchPiece = false;
	}
	//マッチしている
	else if (flame >= EXECUTION_FRAME) {
		boardStatus = BoardStatus::PROCESSING_FLOATCHECK;
		//スコア倍率増加
		scoreScale += 0.5f;

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
	if (flame > 10) {
		boardStatus = BoardStatus::PRICESSING_SPECIALCHECK;
		flame = 0;
	}

}

void Board::TimeControl()
{
	//マッチチェック中は時間進行しない
	//本来はコンボ進行中のみ進行しない
	if (boardStatus == BoardStatus::PROCESSING_MATCHCHECK
		|| boardStatus == BoardStatus::PRICESSING_SPECIALCHECK) {
		return;
	}

	flameCount++;

	if (flameCount > generateRemain && boardStatus == BoardStatus::WAIT) {
		boardStatus = BoardStatus::PROCESSING_GENERATE;
	}

}

void Board::ActiveMatch3(bool isEnforce)
{
	if (isEnforce) {
		for (auto& m : isMatch3Active) {
			m = true;
		}
		return;
	}

	if (level < 5) {
		if (std::find_if(isMatch3Active.begin(), isMatch3Active.end() - 4, [](bool f) { return f; }) == isMatch3Active.end() - 4) {
			for (auto& m : isMatch3Active) {
				m = true;
			}
		}
	}
	else if (level < 10) {
		if (std::find_if(isMatch3Active.begin(), isMatch3Active.end() - 1, [](bool f) { return f; }) == isMatch3Active.end() - 1) {
			for (auto& m : isMatch3Active) {
				m = true;
			}
		}
	}
	else {
		if (std::find_if(isMatch3Active.begin(), isMatch3Active.end(), [](bool f) { return f; }) == isMatch3Active.end()) {
			for (auto& m : isMatch3Active) {
				m = true;
			}
		}
	}


}

void Board::EnableMatch3(PIECE_COLOR color)
{
	switch (color)
	{
	case PIECE_COLOR::PCOLOR_NONE:
		break;
	case PIECE_COLOR::PCOLOR_RED:
		isMatch3Active[0] = true;
		break;
	case PIECE_COLOR::PCOLOR_GREEM:
		isMatch3Active[1] = true;
		break;
	case PIECE_COLOR::PCOLOR_BLUE:
		isMatch3Active[2] = true;
		break;
	case PIECE_COLOR::PCOLOR_WHITE:
		isMatch3Active[3] = true;
		break;
	case PIECE_COLOR::PCOLOR_YELLOW:
		isMatch3Active[4] = true;
		break;
	case PIECE_COLOR::PCOLOR_SKY:
		isMatch3Active[5] = true;
		break;
	case PIECE_COLOR::PCOLOR_PURPLE:
		isMatch3Active[6] = true;
		break;
	case PIECE_COLOR::PCOLOR_BLACK:
		isMatch3Active[7] = true;
		break;
	default:
		break;
	}
}

void Board::DisableMatch3Active(PIECE_COLOR color)
{

	switch (color)
	{
	case PIECE_COLOR::PCOLOR_NONE:
		break;
	case PIECE_COLOR::PCOLOR_RED:
		isMatch3Active[0] = false;
		break;
	case PIECE_COLOR::PCOLOR_GREEM:
		isMatch3Active[1] = false;
		break;
	case PIECE_COLOR::PCOLOR_BLUE:
		isMatch3Active[2] = false;
		break;
	case PIECE_COLOR::PCOLOR_WHITE:
		isMatch3Active[3] = false;
		break;
	case PIECE_COLOR::PCOLOR_YELLOW:
		isMatch3Active[4] = false;
		break;
	case PIECE_COLOR::PCOLOR_SKY:
		isMatch3Active[5] = false;
		break;
	case PIECE_COLOR::PCOLOR_PURPLE:
		isMatch3Active[6] = false;
		break;
	case PIECE_COLOR::PCOLOR_BLACK:
		isMatch3Active[7] = false;
		break;
	default:
		break;
	}
}

bool Board::CheckMatch3Active(PIECE_COLOR color)
{
	bool result = true;
	switch (color)
	{
	case PIECE_COLOR::PCOLOR_NONE:
		result = isMatch3Active[0];
		break;
	case PIECE_COLOR::PCOLOR_RED:
		result = isMatch3Active[0];
		break;
	case PIECE_COLOR::PCOLOR_GREEM:
		result = isMatch3Active[1];
		break;
	case PIECE_COLOR::PCOLOR_BLUE:
		result = isMatch3Active[2];
		break;
	case PIECE_COLOR::PCOLOR_WHITE:
		result = isMatch3Active[3];
		break;

		//以下は一定のレベルで有効化される

	case PIECE_COLOR::PCOLOR_YELLOW:

		if (level >= 5) { result = isMatch3Active[4]; }
		else { result = false; }
		break;
	case PIECE_COLOR::PCOLOR_SKY:
		if (level >= 5) { result = isMatch3Active[5]; }
		else { result = false; }
		break;
	case PIECE_COLOR::PCOLOR_PURPLE:
		if (level >= 5) { result = isMatch3Active[6]; }
		else { result = false; }
		break;
	case PIECE_COLOR::PCOLOR_BLACK:
		if (level >= 10) { result = isMatch3Active[7]; }
		else { result = false; }
		break;
	default:
		break;
	}

	return result;
}

void Board::DrawBoardGrid()
{


	//横線
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		int lx = PieceData::DRAWBASE_X - (PieceData::PIECE_SIZE / 2);
		int rx = PieceData::DRAWBASE_X + (BOARD_WIDTH) * PieceData::PIECE_SIZE - (PieceData::PIECE_SIZE / 2);
		int y = PieceData::DRAWBASE_Y - (PieceData::PIECE_SIZE * i) + (PieceData::PIECE_SIZE / 2);
		int color = 0;
		if (i == BOARD_HEIGHT - 1) {
			color = GetColor(255, 100, 100);
		}
		else {
			color = GetColor(155, 0, 155);
		}
		int LW = LINE_WIDTH;
		if (i == 0 || i == BOARD_HEIGHT - 1) {
			LW = LW + 1;
		}
		
		DrawBoxAA(lx, y - LW, rx, y + LW, color, true);
	}
	//縦線
	for (int j = 0; j < BOARD_WIDTH + 1; j++) {
		int ty = PieceData::DRAWBASE_Y - (PieceData::PIECE_SIZE * (BOARD_HEIGHT -2)) - (PieceData::PIECE_SIZE / 2);
		int by = PieceData::DRAWBASE_Y + (PieceData::PIECE_SIZE / 2);
		int x = PieceData::DRAWBASE_X + (PieceData::PIECE_SIZE * j) - (PieceData::PIECE_SIZE / 2);
		int color = GetColor(155, 0, 155);
		int LW = LINE_WIDTH;
		if (j == 0 || j == BOARD_WIDTH) {
			LW = LW + 1;
		}

		DrawBoxAA(x - LW, ty, x + LW, by, color, true);
	}

	//マスのチェック柄




}
