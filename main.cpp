#include "DxLib.h"

#include "SceneMgr.h"

#include "Board.h"
#include "PlayerControl.h"
#include "Input.h"

#include "FPSControl.h"

#include <wingdi.h>

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "GAMEJAM";

// �E�B���h�E����
const int WIN_WIDTH = 900;

// �E�B���h�E�c��
const int WIN_HEIGHT = 506;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

#ifdef _DEBUG
	//�f�o�b�O�p��ʃT�C�Y
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

#else
	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

#endif // DEBUG

	SetWaitVSyncFlag(false);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(55, 0x00, 55);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	SceneMgr mgr;
	mgr.Initialize();
	FPSControl fps;
	fps.FpsControll_Initialize();
	//�t�H���g�f�[�^�ꎞ�I�ɒǉ�
	AddFontResourceEx("Resources/03�X�}�[�g�t�H���gUI.otf", FR_PRIVATE, NULL);

	// �Q�[�����[�v
	while (1)
	{

		// ��ʃN���A
		ClearDrawScreen();

		Input::Update();
		//---------  ��������v���O�������L�q  ----------//

		mgr.Update();

		mgr.Draw();

		fps.FpsControll_Update();
		fps.FpsControll_Draw();
		fps.FpsControll_Wait();
		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1)
		{
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}