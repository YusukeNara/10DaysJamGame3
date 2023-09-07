#include "Menu.h"
#include "DxLib.h"
#include "Input.h"

Menu::Menu(ISceneChanger *changer) : BaseScene(changer) {
}

//������
void Menu::Initialize() {

}

//�X�V
void Menu::Update() {
	if (Input::IsKeyTrigger(KEY_INPUT_SPACE)) {
		mSceneChanger->ChangeScene(eScene_Game);
	}
}

//�`��
void Menu::Draw() {

}