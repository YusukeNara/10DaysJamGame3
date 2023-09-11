#pragma once

#include <DxLib.h>
#include <vector>

#include "RVector.h"

//�s�[�X���Ŏ��̃p�[�e�B�N�����Đ�
class PieceDeleteDirection
{
private:
	//�����N���X�A�s�[�X���Ńp�[�e�B�N���̎���
	class PieceDeleteDirectionEmiter
	{
	public:
		PieceDeleteDirectionEmiter() = default;
		~PieceDeleteDirectionEmiter() = default;

		void Init(RVector3 start, RVector3 end, int graphHandle, int frame);

		void Update();

		void Draw();

		bool isEnd() { return bezierEase.isEnded(); }

	private:
		Rv3Ease::Rv3Bezier3 bezierEase;
		int graphHandle = 0;

		int size = 0;
	};


public:
	PieceDeleteDirection() = default;
	~PieceDeleteDirection() = default;

	void Init();

	void Update();

	void Draw();

	void PlayEase(int sx, int sy, int ex, int ey, int frame,int generateNum = 1);

private:
	//�����N���X�R���e�i
	std::vector<PieceDeleteDirectionEmiter> emiter;

	//�摜�n���h��

};

