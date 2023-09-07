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

	//�s�[�X�̍��W�f�[�^���㏸������
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

	//���̂Ƃ��V��i��\����11��ځj�𒴂���s�[�X�����݂���ꍇ�Q�[���I�[�o�[�t���O����

	//�s�[�X�f�[�^���E�V�t�g����i�㏸�Ɠ��`�j
	std::shift_right(boardData.begin(), boardData.end(), 1);

	//�ŉ��i�ɐV���ȗv�f��������
	std::array<PieceData, 6> initdata;
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
		rotateP[0].SetPos(selectX + 1, selectY);
		boardData[selectY][selectX + 1] = rotateP[0];
		//�E��->�E��
		rotateP[1].SetPos(selectX + 1, selectY + 1);
		boardData[selectY + 1][selectX + 1] = rotateP[1];
		//�E��->����
		rotateP[2].SetPos(selectX, selectY + 1);
		boardData[selectY + 1][selectX] = rotateP[2];
		//����->����
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
	DrawFormatString(0, 32, GetColor(255, 255, 255), "space :  piece rotate (�����v���)");
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

	//�������s���s�[�X��ێ�
	std::vector<PieceData*> deletePieceBuff;

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
				//�E�ňقȂ�F��������܂Ń��[�v
				for (int xx = x; xx < BOARD_WIDTH; xx++) {
					if (boardData[y][x] != boardData[y][xx]) { break; }
					rightMatchPiece.push_back(&boardData[y][xx]);
				}
				//��ňقȂ�F��������܂Ń��[�v
				for (int yy = y; yy < BOARD_HEIGHT; yy++) {
					if (boardData[y][x] != boardData[yy][x]) { break; }
					upperMatchPiece.push_back(&boardData[yy][x]);
				}

				//���ꂼ��3�ȏ゠��ꍇ�A�����R���e�i�ɕێ�
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

		//�s�[�X�������J�n
		for (auto& d : deletePieceBuff) {
			d->Clear();
			//�s�[�X1�ɂ����Z�X�R�A����
			addScore += (baseScore * scoreScale);
		}
		score += addScore;

		//�X�R�A������l�𒴂����ꍇ�A���x���A�b�v
		if (score > level * 10000) {
			level++;
			if (spawnTime > SPAWNTIME_MIN) {
				spawnTime -= spawnDifficlutyRate;
			}
		}
	}

	flame++;

	if (flame > 15) {
		//�}�b�`���Ă���s�[�X������ꍇ�͕��V�`�F�b�N
		if (isMatchPiece) { 
			boardStatus = BoardStatus::PROCESSING_FLOATCHECK; 
			//�X�R�A�{������
			scoreScale += 0.2f;
		}
		//�����łȂ��Ȃ�ҋ@
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
	if (flame > 5) {
		boardStatus = BoardStatus::PROCESSING_MATCHCHECK;
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

	if (flameCount > spawnTime) {
		UpAndGenerate();
	}

}
