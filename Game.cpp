#include "Game.h"
#include "DxLib.h"


Game::Game(ISceneChanger *changer) : BaseScene(changer) {


}

//初期化
void Game::Initialize() {
    board.Init();
    player.Init(&board);
}

//更新
void Game::Update() {
    if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) { //Escキーが押されていたら
        mSceneChanger->ChangeScene(eScene_Menu);//シーンをメニューに変更
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

//描画
void Game::Draw() {
	board.Draw();
	board.DebugDraw();
	player.Draw();
}
