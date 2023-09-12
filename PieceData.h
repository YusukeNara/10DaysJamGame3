#pragma once

#include "RVector.h"

enum class PIECE_COLOR
{
	PCOLOR_NONE,
	PCOLOR_RED,
	PCOLOR_GREEM,
	PCOLOR_BLUE,
	PCOLOR_WHITE,
};

class PieceData
{
public:

	PieceData();
	~PieceData();

	void Init(int x, int y);

	//�s�[�X�𐶐�
	void Generate(int x,int y);

	//�s�[�X���㏸
	void Up();

	void Down();

	//�s�[�X���W�ݒ�
	void RotatePiece(int newX, int newY);

	void SetPos(int newX, int newY);

	int GetX() { return posX; }
	int GetY() { return posY; }

	void DeleteReservation();
	//�s�[�X����
	void Clear();

	//�`��
	void Draw();

	void DisplayPieceInfo(int displayX, int displayY);

	PIECE_COLOR GetColorNum() { return color; }

	//�`�捶�[�
	static int DRAWBASE_X;
	//�`�扺�
	static int DRAWBASE_Y;
	//�s�[�X�`��T�C�Y
	static const int PIECE_SIZE = 40;
	//�C�[�W���O�t���[����
	static const int EASE_MOVE_TIME = 10;

	// ==���Z�q���I�[�o�[���[�h�i���F�̃s�[�X�𓙉����Z�q�Ŕ��f����j
	bool operator==(const PieceData& r) const
	{
		return color == r.color;
	}
	// !=���Z�q���I�[�o�[���[�h�i���F�̃s�[�X�𓙉����Z�q�Ŕ��f����j
	bool operator!=(const PieceData& r) const
	{
		return !(color == r.color);
	}

	bool GetDeleteReserved() { return isDeleteReserved; }

	int GetDrawCenterX() { return DRAWBASE_X + posX * PIECE_SIZE; };
	int GetDrawCenterY() { return DRAWBASE_Y - posY * PIECE_SIZE; };

private:
	int posX = 0;
	int posY = 0;
	int oldPosX = 0;
	int oldPosY = 0;
	//���������\��̃s�[�X�i�s�[�X�������Ƃ��̔���Ɏg�p���Ȃ��t���O�j
	int isDeleteReserved = false;

	//�x�W�G�C�[�W���O
	Rv3Ease::Rv3Bezier3 bezierEase;

	PIECE_COLOR color = PIECE_COLOR::PCOLOR_NONE;
};