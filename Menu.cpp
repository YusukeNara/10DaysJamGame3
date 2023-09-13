#include "Menu.h"
#include "DxLib.h"
#include "Input.h"

Menu::Menu(ISceneChanger *changer) : BaseScene(changer) {
}

//������
void Menu::Initialize() {
	fontHandle[0] = CreateFontToHandle("03SmartFontUI", 48, -1, DX_FONTTYPE_ANTIALIASING);
	fontHandle[1] = CreateFontToHandle("03SmartFontUI", 52, -1, DX_FONTTYPE_ANTIALIASING);
	renditionTime = 0;
	LoadDivGraph("Resources/pieces.png", 8, 8, 1, 32, 32, piece, false);

	ui.Init();
	sceneChange = false;
}

//�X�V
void Menu::Update() {
	if (Input::IsKeyTrigger(KEY_INPUT_SPACE)) {
		sceneChange = true;
	}
	if (sceneChange) {
		ui.ChangeSceneIn(mSceneChanger);
	}
	renditionTime++;
	rotationTime++;
	if (renditionTime > 60) {
		renditionTime = 0;
	}

	if (Input::IsKeyTrigger(KEY_INPUT_T)) {
		mSceneChanger->ChangeScene(eScene::eScene_Config);
	}

}

//�`��
void Menu::Draw() {

	DrawRotaGraph(400, 450, 2, PI / 180 * (rotationTime * 3), piece[0], true, false);
	DrawRotaGraph(200, 400, 2, PI / 180 * (rotationTime * 3), piece[1], true, false);
	DrawRotaGraph(160, 200, 2, PI / 180 * (rotationTime * 3), piece[2], true, false);
	DrawRotaGraph(700, 50, 2, PI / 180 * (rotationTime * 3), piece[3], true, false);
	DrawRotaGraph(600, 450, 2, PI / 180 * (rotationTime * 3), piece[4], true, false);
	DrawRotaGraph(300, 250, 2, PI / 180 * (rotationTime * 3), piece[5], true, false);
	DrawRotaGraph(500, 250, 2, PI / 180 * (rotationTime * 3), piece[6], true, false);
	DrawRotaGraph(800, 200, 2, PI / 180 * (rotationTime * 3), piece[7], true, false);

	DrawFormatStringToHandle(200, 100, GetColor(255, 255, 255), fontHandle[0], "�Ȃ�ԁE�܂��E�΂Ԃ�");
	if (renditionTime < 30) {
		DrawFormatStringToHandle(240, 300, GetColor(255, 255, 255), fontHandle[0], "SPACE�ŃX�^�[�g");
		DrawFormatStringToHandle(240, 360, GetColor(255, 255, 255), fontHandle[0], "T�Ń`���[�g���A��");
	}
	else {
		DrawFormatStringToHandle(236, 298, GetColor(255, 255, 255), fontHandle[1], "SPACE�ŃX�^�[�g");
		DrawFormatStringToHandle(238, 360, GetColor(255, 255, 255), fontHandle[1], "T�Ń`���[�g���A��");
	}
	//DrawFormatString(200, 400, GetColor(255, 255, 255), "a");

	
	if (sceneChange) {
		ui.DrawChangeIn();
	}
}