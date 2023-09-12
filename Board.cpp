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
	ui.Init();
	level = 1;
	flameCount = 0;
	generateRemain = SPAWNTIME_MAX;

	for (int i = 0; i < 3; i++) {
		UpAndGenerate();
	}

	//���\�[�X�ǂݍ���
	bigFontHandle = CreateFontToHandle("03�X�}�[�g�t�H���gUI", 64, 9, DX_FONTTYPE_NORMAL);
	smallFontHandle = CreateFontToHandle("03�X�}�[�g�t�H���gUI", 16, 6, DX_FONTTYPE_NORMAL);
	//fontHandle = LoadFontDataToHandle("Resources/SmartFontUI.dft", 0);

	pieceDeleteDirection.Init();
	pieceDeleteTextDrawer.Init();
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
		break;
	default:
		break;
	}

	ui.Update();

	//���o�n�X�V
	pieceDeleteDirection.Update();
	pieceDeleteTextDrawer.Update();

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

	ui.DrawFlame();
	ui.DrawLevel(level);
	ui.DrawScore(score);
	ui.DrawTime((generateRemain - flameCount) / 60u);
	DrawBoardGrid();

	if (boardStatus == BoardStatus::GAMEOVER) {
		SetFontSize(48);
		SetFontThickness(9);
		//�^�񒆂ɃQ�[���I�[�o�[�\��
		DrawStringToHandle(PieceData::DRAWBASE_X-64, 300, "GAME OVER", GetColor(255, 255, 255), bigFontHandle);
		SetFontThickness(6);
		SetFontSize(16);
	}

	pieceDeleteDirection.Draw();
	pieceDeleteTextDrawer.Draw();
}

void Board::UpAndGenerate()
{
	if (boardStatus == BoardStatus::GAMEOVER) { return; }

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
	
	int x = 0;
	for (auto& b : boardData[0]) {
		b.Generate(x, 0);
		x++;
	}

	//�������ɐ����Ԋu�����Z�b�g
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

	/*DrawFormatString(0, 0, GetColor(255, 255, 255), "x %d  y %d", mouseX, mouseY);
	DrawFormatString(0, 16, GetColor(255, 255, 255), "WASD :   box move");
	DrawFormatString(0, 32, GetColor(255, 255, 255), "space :  piece rotate (�����v���)");
	DrawFormatString(0, 48, GetColor(255, 255, 255), "U :  piece generate");
	DrawFormatString(0, 64, GetColor(255, 255, 255), "R :  reset");

	DrawFormatString(0, 96, GetColor(255, 255, 255), "score : %u", score);
	DrawFormatString(0, 112, GetColor(255, 255, 255), "generate remain : %u", (generateRemain - flameCount) / 60u);
	DrawFormatString(0, 128, GetColor(255, 255, 255), "generate rate : %u", generateRemain / 60u);
	DrawFormatString(0, 144, GetColor(255, 255, 255), "level : %u", level);*/
	if (boardStatus == BoardStatus::GAMEOVER) {
		DrawFormatString(0, 160, GetColor(255, 255, 255), "GAME OVER");
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

				//���ꂼ��5�ȏ゠��ꍇ�A�����R���e�i�ɕێ�
				if (upperMatchPiece.size() >= 5) {
					int count = 0;
					for (auto& up : upperMatchPiece) {
						up->DeleteReservation();
						deletePieceBuff.push_back(up);
						count++;
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
					isMatchPiece = true;
					isMatchStraight = true;
				}
				if (isMatchStraight) { break; }
			}
			if (isMatchStraight) { break; }
		}

		//�}�b�`������X�L�b�v����
		if (isMatchStraight) {
			
		}

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
					//���F�̃s�[�X�����ׂč폜���ă��[�v������
					for (auto& group : matchPiece) {
						group->DeleteReservation();
						deletePieceBuff.push_back(group);
					}
					for (auto& py : boardData) {
						for (auto& px : py) {
							if (px == *deletePieceBuff[0] && !px.GetDeleteReserved()) {
								deletePieceBuff.push_back(&px);
							}
						}
					}
					break;
				}
			}
			if (isMatchDiamond) { break; }
		}

		//�r�N�g���[�`�F�b�N
		for (int y = 0; y < BOARD_HEIGHT - 3; y++) {
			for (int x = 2; x < BOARD_WIDTH - 2; x++) {
				if (boardData[y][x].GetColorNum() == PIECE_COLOR::PCOLOR_NONE) {
					continue;
				}
				//��̃s�[�X���ێ�
				std::vector<PieceData*> ;

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
					//���F�̃s�[�X�����ׂč폜���ă��[�v������
					for (auto& group : matchPiece) {
						group->DeleteReservation();
						deletePieceBuff.push_back(group);
					}
					for (auto& py : boardData) {
						for (auto& px : py) {
							if (px == *deletePieceBuff[0] && !px.GetDeleteReserved()) {
								deletePieceBuff.push_back(&px);
							}
						}
					}
					break;
				}

			}
			if (isMatchVictory) { break; }
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

	//�Y������X�y�V�����}�b�`�ɉ��������o�Đ�
	if (isMatchStraight) {
		addScore += 5000;
		//�X�g���[�g���o�Đ�
		pieceDeleteTextDrawer.DrawSpecialText(0);
		combo++;
	}
	if (isMatchDiamond) {
		addScore += 5000;
		//�X�g���[�g���o�Đ�
		pieceDeleteTextDrawer.DrawSpecialText(1);
		combo++;
	}
	if (isMatchVictory) {
		addScore += 5000;
		//�X�g���[�g���o�Đ�
		pieceDeleteTextDrawer.DrawSpecialText(2);
		combo++;
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

			d->Clear();
			//�s�[�X1�ɂ����Z�X�R�A����
			addScore += (baseScore * scoreScale);
		}
		int oldScore;
		oldScore = score;
		score += addScore;
		if (oldScore != score) {
			ui.AddScore(addScore);
		}

		//�X�R�A������l�𒴂����ꍇ�A���x���A�b�v
		if (score > level * 10000) {
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
	if (boardStatus == BoardStatus::PROCESSING_MATCHCHECK) {
		return;
	}

	flameCount++;

	if (flameCount > generateRemain) {
		UpAndGenerate();
	}

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
