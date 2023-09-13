#include "Game.h"
#include "DxLib.h"


Game::Game(ISceneChanger *changer) : BaseScene(changer) {


}

//������
void Game::Initialize() {
    board.Init();
    player.Init(&board);
}

//�X�V
void Game::Update() {
    if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) { //Esc�L�[��������Ă�����
        mSceneChanger->ChangeScene(eScene_Menu);//�V�[�������j���[�ɕύX
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

//�`��
void Game::Draw() {
	board.Draw();
	board.DebugDraw();
	player.Draw();
}
