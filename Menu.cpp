#include "Menu.h"
#include "DxLib.h"
#include "Input.h"

Menu::Menu(ISceneChanger *changer) : BaseScene(changer) {
}

//‰Šú‰»
void Menu::Initialize() {
	fontHandle = LoadFontDataToHandle("Resources/SmartFontUI.dft", 0);
}

//XV
void Menu::Update() {
	if (Input::IsKeyTrigger(KEY_INPUT_SPACE)) {
		mSceneChanger->ChangeScene(eScene_Game);
	}


}

//•`‰æ
void Menu::Draw() {

	DrawFormatStringToHandle(0, 0, GetColor(255, 255, 255), fontHandle, "STROTATE");
}