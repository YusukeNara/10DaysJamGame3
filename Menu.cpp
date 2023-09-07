#include "Menu.h"
#include "DxLib.h"
#include "Input.h"

Menu::Menu(ISceneChanger *changer) : BaseScene(changer) {
}

//初期化
void Menu::Initialize() {

}

//更新
void Menu::Update() {
	if (Input::IsKeyTrigger(KEY_INPUT_SPACE)) {
		mSceneChanger->ChangeScene(eScene_Game);
	}
}

//描画
void Menu::Draw() {

}