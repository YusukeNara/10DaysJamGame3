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

	//���\�[�X�ǂݍ���
	bigFontHandle = CreateFontToHandle("03�X�}�[�g�t�H���gUI", 64, 9, DX_FONTTYPE_NORMAL);
	smallFontHandle = CreateFontToHandle("03�X�}�[�g�t�H���gUI", 16, 6, DX_FONTTYPE_NORMAL);
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
	//���Ԑi�s
	TimeControl();

	//�X�e�[�^�X�ɉ����Ė��t���[���֐������s
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

	//���ׂẴ}�b�`3�����������ꂽ��ēx�L����
	ActiveMatch3();

	ui.Update();

	//���o�n�X�V
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
		//�s�[�X�̍��W�f�[�^���㏸������
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

		//���̂Ƃ��V��i��\����11��ځj�𒴂���s�[�X�����݂���ꍇ�Q�[���I�[�o�[�t���O����

		//�s�[�X�f�[�^���E�V�t�g����i�㏸�Ɠ��`�j
		std::shift_right(boardData.begin(), boardData.end(), 1);

		//�ŉ��i�ɐV���ȗv�f��������
		std::array<PieceData, BOARD_WIDTH> initdata;
		boardData[0] = initdata;

		//�{�[�h��̃s�[�X�̐�
		std::array<int, 8> pcolorCount{ 0,0,0,0,0,0,0,0 };

		for (auto& y : boardData) {
			//PCOLOR_NUM�̏��Ԃɔz��ɕۑ�
			pcolorCount[0] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_RED; });
			pcolorCount[1] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_GREEM; });
			pcolorCount[2] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_BLUE; });
			pcolorCount[3] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_WHITE; });
			pcolorCount[4] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_YELLOW; });
			pcolorCount[5] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_SKY; });
			pcolorCount[6] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_PURPLE; });
			pcolorCount[7] += std::count_if(y.begin(), y.end(), [](PieceData p) {return p.GetColorNum() == PIECE_COLOR::PCOLOR_BLACK; });
		}

		//��ԏ��Ȃ��s�[�X�ԍ���ێ�

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
			//���x���ɉ����Đ�������s�[�X���قȂ�
			if (level < 5) {
				int colorNum = NY_random::intrand_sl(4, 1);
				const int MIN_PIECE_GENERATE_RATE = 75;
				if (NY_random::intrand_sl(100, 0) < MIN_PIECE_GENERATE_RATE) {
					//�{�[�h���ōł����Ȃ��s�[�X�𐶐�
					colorNum = minPiece + 1;
					//���������F���[���͓K�p
					//�㍶�œ��F�𐶐����Ȃ�
					if (x == 0) {
						//��2�}�X���������F�̂Ƃ��͍ēx����
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
						//��2�}�X���������F�̂Ƃ��͍ēx����
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
				//�ŏ��s�[�X�����m��
				const int MIN_PIECE_GENERATE_RATE = 75;
				if (NY_random::intrand_sl(100, 0) < MIN_PIECE_GENERATE_RATE) {
					//�{�[�h���ōł����Ȃ��s�[�X�𐶐�
					colorNum = minPiece + 1;
					//���������F���[���͓K�p
					//�㍶�œ��F�𐶐����Ȃ�
					if (x == 0) {
						//��2�}�X���������F�̂Ƃ��͍ēx����
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
					//�㍶�œ��F�𐶐����Ȃ�
					if (x == 0) {
						//��2�}�X���������F�̂Ƃ��͍ēx����
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
				//�s�[�X�̑������v�Z���A�ł����Ȃ��s�[�X�̐��������グ��
				int colorNum = 0;
				//�ŏ��s�[�X�����m��
				const int MIN_PIECE_GENERATE_RATE = 75;
				if (NY_random::intrand_sl(100, 0) < MIN_PIECE_GENERATE_RATE) {
					//�{�[�h���ōł����Ȃ��s�[�X�𐶐�
					colorNum = minPiece + 1;
					//���������F���[���͓K�p
					//�㍶�œ��F�𐶐����Ȃ�
					if (x == 0) {
						//��2�}�X���������F�̂Ƃ��͍ēx����
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
					//�㍶�œ��F�𐶐����Ȃ�
					if (x == 0) {
						//��2�}�X���������F�̂Ƃ��͍ēx����
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

	//�I�𒆂̃s�[�X4��ێ��i�����A�E���A�E��A����j
	if (flame == 0) {
		std::array<PieceData, 4> rotateP = {
		boardData[selectY][selectX],
		boardData[selectY][selectX + 1],
		boardData[selectY + 1][selectX + 1],
		boardData[selectY + 1][selectX] };

		//����->�E��
		rotateP[0].RotatePiece(selectX + 1, selectY);
		boardData[selectY][selectX + 1] = rotateP[0];
		//�E��->�E��
		rotateP[1].RotatePiece(selectX + 1, selectY + 1);
		boardData[selectY + 1][selectX + 1] = rotateP[1];
		//�E��->����
		rotateP[2].RotatePiece(selectX, selectY + 1);
		boardData[selectY + 1][selectX] = rotateP[2];
		//����->����
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
	//�����t���[��
	const int EXECUTION_FRAME = 60;

	//�X�g���[�g�}�b�`�t���O
	bool isMatchStraight = false;
	//�_�C�������h�}�b�`�t���O
	bool isMatchDiamond = false;
	//�r�N�g���[�}�b�`�t���O
	bool isMatchVictory = false;
	//�}�b�`���m�F
	static bool isMatchPiece = false;

	int addScore = 0;

	//��������s�[�X�܂Ƃ�
	std::vector<PieceData*> deletePieceBuff;

	if (flame == 0) {
		//�X�g���[�g�`�F�b�N
		//��������`�F�b�N
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			for (int x = 0; x < BOARD_WIDTH; x++) {
				if (boardData[y][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) {
					continue;
				}
				//��̃s�[�X���ێ�
				std::vector<PieceData*> upperMatchPiece;
				//�E�̃s�[�X���ێ�
				std::vector<PieceData*> rightMatchPiece;
				//�E�ňقȂ�F��������or�����ό��m�܂Ń��[�v
				for (int xx = x; xx < BOARD_WIDTH; xx++) {
					if (boardData[y][x] != boardData[y][xx]
						|| boardData[y][xx].GetDeleteReserved()) {
						break;
					}
					rightMatchPiece.push_back(&boardData[y][xx]);
				}
				//��ňقȂ�F��������or�����ό��m�܂Ń��[�v
				for (int yy = y; yy < BOARD_HEIGHT; yy++) {
					if (boardData[y][x] != boardData[yy][x]
						|| boardData[yy][x].GetDeleteReserved()) {
						break;
					}
					upperMatchPiece.push_back(&boardData[yy][x]);
				}

				//���ꂼ��5�ȏ゠��ꍇ�A�������s�[�X�Ɠ��F�s�[�X�������R���e�i�ɕێ�
				if (upperMatchPiece.size() >= 5) {
					int count = 0;
					for (auto& up : upperMatchPiece) {
						up->DeleteReservation();
						deletePieceBuff.push_back(up);
						count++;
					}
					//���F�s�[�X������
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
					//���F�s�[�X������
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

		//�}�b�`������X�L�b�v����
		if (!isMatchStraight) {
			//�_�C�������h�`�F�b�N
			for (int y = 0; y < BOARD_HEIGHT - 4; y++) {
				//�_�C�������h�͒[���猟�m���Ȃ�
				for (int x = 1; x < BOARD_WIDTH - 1; x++) {	//
					if (boardData[y][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) {
						continue;
					}
					//�����s�[�X�̂܂Ƃ܂��ێ�
					std::vector<PieceData*> matchPiece;
					if (boardData[y][x] == boardData[y + 3][x] &&
						boardData[y][x] == boardData[y + 2][x - 1] &&
						boardData[y][x] == boardData[y + 1][x - 1] &&
						boardData[y][x] == boardData[y + 2][x + 1] &&
						boardData[y][x] == boardData[y + 1][x + 1]) {
						//�Y������ꏊ���폜�o�b�t�@�ɕۑ�
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
						//���ׂĂ̐F�������\��
						ActiveMatch3(true);
						
						break;
					}
				}
				if (isMatchDiamond) { break; }
			}
		}

		if (!isMatchDiamond) {
			//�r�N�g���[�`�F�b�N
			for (int y = 0; y < BOARD_HEIGHT - 3; y++) {
				for (int x = 2; x < BOARD_WIDTH - 2; x++) {
					if (boardData[y][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) {
						continue;
					}
					//�����s�[�X�̂܂Ƃ܂��ێ�
					std::vector<PieceData*> matchPiece;
					if (boardData[y][x] == boardData[y + 2][x - 2] &&
						boardData[y][x] == boardData[y + 1][x - 1] &&
						boardData[y][x] == boardData[y + 2][x + 2] &&
						boardData[y][x] == boardData[y + 1][x + 1]) {
						//�Y������ꏊ���폜�o�b�t�@�ɕۑ�
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
						//���ׂĂ̐F�������\��
						ActiveMatch3(true);
						break;
					}

				}
				if (isMatchVictory) { break; }
			}
		}
	}

	//�s�[�X�������J�n
	for (auto& d : deletePieceBuff) {
		//���o�Đ��ʒu
		int ease_posX = PieceData::DRAWBASE_X +
			d->GetX() * PieceData::PIECE_SIZE +
			PieceData::PIECE_SIZE / 2;
		//���o�Đ��ʒu
		int ease_posY = PieceData::DRAWBASE_Y -
			d->GetY() * PieceData::PIECE_SIZE -
			PieceData::PIECE_SIZE / 2;
		//���o�Đ�
		pieceDeleteDirection.PlayEase(ease_posX, ease_posY, 0, 0, EXECUTION_FRAME,5);
		d->Clear();
		//�s�[�X1�ɂ����Z�X�R�A����
		addScore += (baseScore * scoreScale);
	}

	//�Y������X�y�V�����}�b�`�ɉ������X�R�A���Z�A���o�Đ��A������ʔ���
	if (isMatchStraight) {
		addScore += 5000;
		//�X�g���[�g���o�Đ�
		pieceDeleteTextDrawer.DrawSpecialText(0);
		combo++;
		score += addScore * scoreScale;
		ui.AddScore(addScore);
	}
	if (isMatchDiamond) {
		addScore += 5000;
		//�X�g���[�g���o�Đ�
		pieceDeleteTextDrawer.DrawSpecialText(1);
		combo++;
		score += addScore * scoreScale;
		ui.AddScore(addScore);
	}
	if (isMatchVictory) {
		addScore += 5000;
		//�X�g���[�g���o�Đ�
		pieceDeleteTextDrawer.DrawSpecialText(2);
		combo++;
		score += addScore * scoreScale;
		ui.AddScore(addScore);
	}

	
	flame++;
	//�}�b�`���Ă�s�[�X���Ȃ�
	if (!isMatchPiece) {
		boardStatus = BoardStatus::PROCESSING_MATCHCHECK;
		scoreScale = 1.0f;
		flame = 0;
		isMatchPiece = false;
	}
	//�}�b�`���Ă���
	else if(flame >= EXECUTION_FRAME){
		boardStatus = BoardStatus::PROCESSING_FLOATCHECK;
		//�X�R�A�{������
		scoreScale += 0.5f;

		flame = 0;
		isMatchPiece = false;
	}

}

void Board::CheckMatch()
{
	static int flame = 0;
	//�����t���[��
	const int EXECUTION_FRAME = 30;

	//�������s���s�[�X��ێ�
	std::vector<PieceData*> deletePieceBuff;
	//��������s�[�X�̂܂Ƃ܂��ێ��i�R���{�p�j
	std::vector<std::vector<PieceData*>> deletePieceGroupBuff;

	static bool isMatchPiece = false;

	int addScore = 0;

	if (flame == 0) {
		//��������`�F�b�N
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			for (int x = 0; x < BOARD_WIDTH; x++) {
				if (boardData[y][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) {
					continue;
				}
				if (!CheckMatch3Active(boardData[y][x].GetColorNum())) {
					continue;
				}
				//��̃s�[�X���ێ�
				std::vector<PieceData*> upperMatchPiece;
				//�E�̃s�[�X���ێ�
				std::vector<PieceData*> rightMatchPiece;
				//�E�ňقȂ�F��������or�����ό��m�܂Ń��[�v
				for (int xx = x; xx < BOARD_WIDTH; xx++) {
					if (boardData[y][x] != boardData[y][xx]
						|| boardData[y][xx].GetDeleteReserved()) {
						break;
					}
					rightMatchPiece.push_back(&boardData[y][xx]);
				}
				//��ňقȂ�F��������or�����ό��m�܂Ń��[�v
				for (int yy = y; yy < BOARD_HEIGHT; yy++) {
					if (boardData[y][x] != boardData[yy][x]
						|| boardData[yy][x].GetDeleteReserved()) {
						break;
					}
					upperMatchPiece.push_back(&boardData[yy][x]);
				}

				//���ꂼ��3�ȏ゠��ꍇ�A�����R���e�i�ɕێ�
				if (upperMatchPiece.size() >= 3) {
					//�R���{���Z
					combo++;
					for (auto& up : upperMatchPiece) { 
						deletePieceBuff.push_back(up); 
						up->DeleteReservation();
					}
					//�R���{�\��
					pieceDeleteTextDrawer.DrawComboText((*upperMatchPiece.begin())->GetDrawCenterX(),
						(*upperMatchPiece.begin())->GetDrawCenterY(),
						EXECUTION_FRAME,
						combo);

					isMatchPiece = true;
				}
				if (rightMatchPiece.size() >= 3) {
					//�R���{���Z
					combo++;
					for (auto& right : rightMatchPiece) { 
						right->DeleteReservation();
						deletePieceBuff.push_back(right); 
					}
					//�R���{�\��
					pieceDeleteTextDrawer.DrawComboText((*rightMatchPiece.begin())->GetDrawCenterX(),
						(*rightMatchPiece.begin())->GetDrawCenterY(),
						EXECUTION_FRAME,
						combo);
					isMatchPiece = true;
				}
			}
		}

		//�s�[�X�������J�n
		for (auto& d : deletePieceBuff) {
			//���o�Đ��ʒu
			int ease_posX = PieceData::DRAWBASE_X + 
				d->GetX() * PieceData::PIECE_SIZE + 
				PieceData::PIECE_SIZE / 2;
			//���o�Đ��ʒu
			int ease_posY = PieceData::DRAWBASE_Y -
				d->GetY() * PieceData::PIECE_SIZE -
				PieceData::PIECE_SIZE / 2;
			//���o�Đ�
			pieceDeleteDirection.PlayEase(ease_posX, ease_posY, 0, 0, EXECUTION_FRAME);

			//�������F�̏����\�t���O���A�N�e�B�u��
			DisableMatch3Active(d->GetColorNum());

			//�s�[�X����
			d->Clear();

			//�s�[�X1�ɂ����Z�X�R�A����
			addScore += (baseScore * scoreScale);
		}
		int oldScore;
		oldScore = score;
		score += addScore * scoreScale;
		if (oldScore != score) {
			ui.AddScore(addScore);
		}

		//�X�R�A������l�𒴂����ꍇ�A���x���A�b�v
		if (score > level * 5000) {
			level++;
			ui.LevelUp();
			if (generateRemain > SPAWNTIME_MIN) {
				generateRemain -= spawnDifficlutyRate;
			}
		}
	}

	flame++;
	//�}�b�`���Ă�s�[�X���Ȃ�
	if (!isMatchPiece) {
		boardStatus = BoardStatus::WAIT;
		scoreScale = 1.0f;
		combo = 0;
		flame = 0;
		isMatchPiece = false;
	}
	//�}�b�`���Ă���
	else if (flame >= EXECUTION_FRAME) {
		boardStatus = BoardStatus::PROCESSING_FLOATCHECK;
		//�X�R�A�{������
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
				//���s�[�X�̉��Ƀs�[�X�����݂��邩�A��ԉ��ɂ��܂ŉ����葱����
				while (boardData[newY - 1][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) {
					downFlag = true;
					newY--;
					if (newY == 0) { break; }
				}

				//�Vy���W�̈ʒu�̃s�[�X���ɑ�����A��y���W�̃s�[�X��none��
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
	//�}�b�`�`�F�b�N���͎��Ԑi�s���Ȃ�
	//�{���̓R���{�i�s���̂ݐi�s���Ȃ�
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

		//�ȉ��͈��̃��x���ŗL���������

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


	//����
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
	//�c��
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

	//�}�X�̃`�F�b�N��




}
