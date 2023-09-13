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
	//piece[0] = LoadGraph();
}

//更新
void Menu::Update() {
	if (Input::IsKeyTrigger(KEY_INPUT_SPACE)) {
		mSceneChanger->ChangeScene(eScene_Game);
	}
	renditionTime++;
	rotationTime++;
	if (renditionTime > 60) {
		renditionTime = 0;
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
	DrawFormatString(200, 400, GetColor(255, 255, 255), "a");
	DrawRotaGraph(400, 450, 1, PI / rotationTime,piece[0], true, false);
}