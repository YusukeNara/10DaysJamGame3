#include "Menu.h"
#include "DxLib.h"
#include "Input.h"

Menu::Menu(ISceneChanger *changer) : BaseScene(changer) {
}

//初期化
void Menu::Initialize() {
	fontHandle[0] = CreateFontToHandle("03SmartFontUI", 48, -1, DX_FONTTYPE_ANTIALIASING);
	fontHandle[1] = CreateFontToHandle("03SmartFontUI", 52, -1, DX_FONTTYPE_ANTIALIASING);
	renditionTime = 0;
	LoadDivGraph("Resources/pieces.png", 8, 8, 1, 32, 32, piece, false);

	ui.Init();
	sceneChange = false;
	changeOut = true;
}

//更新
void Menu::Update() {
	if (!changeOut) {
		if (Input::IsKeyTrigger(KEY_INPUT_SPACE)) {
			sceneChange = true;
		}
		if (sceneChange) {
			ui.ChangeSceneIn(mSceneChanger, eScene_Game);
		}
		renditionTime++;
		rotationTime++;
		if (renditionTime > 60) {
			renditionTime = 0;
		}
	}
	else {
		changeOut = ui.ChangeSceneOut();
	}
	

}

//描画
void Menu::Draw() {

	DrawFormatStringToHandle(200, 100, GetColor(255, 255, 255), fontHandle[0], "ならぶ・まわる・ばぶる");
	if (renditionTime < 30) {
		DrawFormatStringToHandle(360, 300, GetColor(255, 255, 255), fontHandle[0], "スタート");
	}
	else {
		DrawFormatStringToHandle(354, 298, GetColor(255, 255, 255), fontHandle[1], "スタート");
	}
	//DrawFormatString(200, 400, GetColor(255, 255, 255), "a");
	DrawRotaGraph(400, 450, 2, PI / 180 * (rotationTime * 3), piece[0], true, false);
	DrawRotaGraph(200, 400, 2, PI / 180 * (rotationTime * 3), piece[1], true, false);
	DrawRotaGraph(160, 200, 2, PI / 180 * (rotationTime * 3), piece[2], true, false);
	DrawRotaGraph(700, 300, 2, PI / 180 * (rotationTime * 3), piece[3], true, false);
	DrawRotaGraph(600, 450, 2, PI / 180 * (rotationTime * 3), piece[4], true, false);
	DrawRotaGraph(300, 250, 2, PI / 180 * (rotationTime * 3), piece[5], true, false);
	DrawRotaGraph(500, 250, 2, PI / 180 * (rotationTime * 3), piece[6], true, false);
	DrawRotaGraph(800, 200, 2, PI / 180 * (rotationTime * 3), piece[7], true, false);
	
	if (sceneChange) {
		ui.DrawChangeIn();
	}
	if (changeOut) {
		ui.DrawChangeOut();
	}
}