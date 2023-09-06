#pragma once
#include "PieceData.h"

#include <array>

enum class BoardStatus
{
	WAIT,
	PROCESSING_ROTATE,		//�s�[�X��]
	PROCESSING_FLOATCHECK,	//���V�`�F�b�N
	PROCESSING_MATCHCHECK,	//�s�[�X�}�b�`�`�F�b�N
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
	//���Z�b�g����鎞��
	unsigned int resetTime = 3000;
	//���Z�b�g����鎞�Ԃ̌�����
	unsigned int mintime = 120;
	//���Z�b�g����鎞�Ԃ̉���
	const unsigned int RESETTIME_MIN = 360;

	//�X�R�A
	unsigned int score = 0;
	//���Z����X�R�A
	unsigned int addScore = 100;
	//�A���{��
	float comboScale = 1.0f;
	//���x���A�b�v�ɕK�v�ȃX�R�A
	unsigned int levelupScore = 10000;
	//���x���A�b�v�ɕK�v�ȃX�R�A�̏㏸��
	unsigned int levelupScoreBase = 10000;

};

