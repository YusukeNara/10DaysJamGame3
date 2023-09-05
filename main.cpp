#include "DxLib.h"

#include "Board.h"
#include "PlayerControl.h"
#include "Input.h"

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "GAMEJAM";

// ウィンドウ横幅
const int WIN_WIDTH = 900;

// ウィンドウ縦幅
const int WIN_HEIGHT = 600;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

#ifdef _DEBUG
	//デバッグ用画面サイズ
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

#else
	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

#endif // DEBUG

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み

	// ゲームループで使う変数の宣言

	Board board;
	board.Init();
	PlayerControl player;
	player.Init(&board);

	// ゲームループ
	while (1)
	{

		// 画面クリア
		ClearDrawScreen();

		Input::Update();


		//---------  ここからプログラムを記述  ----------//

		player.UserControl();

		if (Input::IsKeyTrigger(KEY_INPUT_U)) {
			board.UpAndGenerate();
		}

		if (Input::IsKeyTrigger(KEY_INPUT_R)) {
			board.Init();
		}

		board.Update();

		player.Draw();

		board.Draw();
		board.DebugDraw();


		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}