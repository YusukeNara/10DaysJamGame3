#pragma once
#include "PieceData.h"
#include "PieceDeleteDirection.h"
#include "PieceDeleteTextDrawer.h"

#include <array>

enum class BoardStatus
{
	WAIT,
	PROCESSING_ROTATE,		//�s�[�X��]
	PROCESSING_FLOATCHECK,	//���V�`�F�b�N
	PROCESSING_MATCHCHECK,	//�s�[�X�}�b�`�`�F�b�N
	PRICESSING_SPECIALCHECK,
	GAMEOVER,				//�Q�[���I�[�o�[
};

//�{�[�h�̏�Ԃ��Ǘ�����N���X
class Board
{
public:
	Board();
	~Board();

	void Init();

	void Update();

	void Draw();

	//�s�[�X�㏸
	void UpAndGenerate();

	void StartPieceRotate(int selectX, int selectY);

	//�s�[�X��]�i���t���[�����s�j
	void RotatePiece(int selectX, int selectY);

	void DebugDraw();
	//����}�b�`�`�F�b�N
	void CheckSpecialMatch();
	//�}�b�`�`�F�b�N
	void CheckMatch();
	//���V�`�F�b�N
	void CheckFloat();
	//�Q�[������
	void TimeControl();

	static const int BOARD_WIDTH = 8;
	static const int BOARD_HEIGHT = 11;

	int rotateX = 0, rotateY = 0;

private:
	std::array <std::array<PieceData, BOARD_WIDTH>, BOARD_HEIGHT> boardData;

	BoardStatus boardStatus;

	//���݂̃��x��
	unsigned int level = 1;
	//�t���[���J�E���g
	unsigned int flameCount = 0;
	//�����Ԋu
	unsigned int generateRemain = 600;
	//�����Ԋu�̌�����
	unsigned int spawnDifficlutyRate = 15;
	//�����y�[�X����
	const unsigned int SPAWNTIME_MIN = 150;
	//�����y�[�X�����l
	const unsigned int SPAWNTIME_MAX = 600;

	//�X�R�A
	unsigned int score = 0;
	//�X�R�A�x�[�X
	unsigned int baseScore = 100;
	//�X�R�A�{���i�A���ŏ㏸�j
	float scoreScale = 1.0f;
	//�R���{
	int combo = 0;

	//�X�y�V�����}�b�`���o�Đ��p

	//���\�[�X
	int bigFontHandle = 0;
	int smallFontHandle = 0;

	//���o�n
	PieceDeleteDirection pieceDeleteDirection;
	PieceDeleteTextDrawer pieceDeleteTextDrawer;

	//�}�X�ڂ̃x�[�X��ۑ�

	//�}�X���̑���
	const int LINE_WIDTH = 1;
	//�}�X�ڕ`��
	void DrawBoardGrid();

};

