#include "DxLib.h"

#include "SceneMgr.h"

#include "Board.h"
#include "PlayerControl.h"
#include "Input.h"

#include "FPSControl.h"

#include <wingdi.h>

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "GAMEJAM";

// ウィンドウ横幅
const int WIN_WIDTH = 900;

// ウィンドウ縦幅
const int WIN_HEIGHT = 506;

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

	SetWaitVSyncFlag(false);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(55, 0x00, 55);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み

	// ゲームループで使う変数の宣言
	SceneMgr mgr;
	mgr.Initialize();
	FPSControl fps;
	fps.FpsControll_Initialize();
	//フォントデータ一時的に追加
	AddFontResourceEx("Resources/03スマートフォントUI.otf", FR_PRIVATE, NULL);

	// ゲームループ
	while (1)
	{

		// 画面クリア
		ClearDrawScreen();

		Input::Update();
		//---------  ここからプログラムを記述  ----------//

		mgr.Update();

		mgr.Draw();

		fps.FpsControll_Update();
		fps.FpsControll_Draw();
		fps.FpsControll_Wait();
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