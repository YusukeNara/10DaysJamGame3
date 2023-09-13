#include "Game.h"
#include "DxLib.h"


Game::Game(ISceneChanger *changer) : BaseScene(changer) {


}

//������
void Game::Initialize() {
	flagInit();
	board.Init();
    player.Init(&board);
	ui.Init();
}

//�X�V
void Game::Update() {
	if (!changeOut) {
		if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) { //Esc�L�[��������Ă�����
			changeMenu = true;
		}
		if (changeMenu) {
			ui.ChangeSceneIn(mSceneChanger, eScene_Menu);//�V�[�������j���[�ɕύX
		}

		player.UserControl();
		if (Input::IsKeyTrigger(KEY_INPUT_U)) {
			board.UpAndGenerate(true);
		}
		if (Input::IsKeyTrigger(KEY_INPUT_R)) {
			board.Init();
		}

		board.Update();
	}
	else {
		changeOut = ui.ChangeSceneOut();
	}

	ui.Update();
}

//�`��
void Game::Draw() {
	
	board.Draw();
	board.DebugDraw();
	player.Draw();
	if (changeOut) {
		ui.DrawChangeOut();
	}
	if (changeMenu) {
		ui.DrawChangeIn();
	}
}

void Game::flagInit()
{
	changeOut = true;
	changeMenu = false;
}
