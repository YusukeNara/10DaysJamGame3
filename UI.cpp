#include "UI.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::Init()
{
	addScore = 0;
	scoreRenditionTime = 0;
	levelUpRenditionTime = 0;
	changeSceneTime = 0;
	isLevelUp = false;
	isRendition = false;

	renditionColor = GetColor(255, 239, 108);
	whiteColor = GetColor(255, 255, 255);
	for (int i = 0; i < fontQuantity; i++) {
		fontData[i] = 0;
	}
	for (int i = 0; i < pieceCount; i++) {
		pieceData[i] = 0;
	}

	CreateFontData();
	CreateResourceData();
}

void UI::Update()
{
	if (isRendition == true) {
		AddScoreRendition();
	}
	if (isLevelUp == true) {
		LevelUpRendition();
	}

}

void UI::AddScore(int addScore)
{
	this->addScore = addScore;
	isScoRend = true;
	isRendition = true;
}

void UI::LevelUp()
{
	isLevelUp = true;
	isLevRend = true;
}

void UI::DrawFlame()
{
	/*DrawBox(0, 0, 310, 506, GetColor(100, 100, 100), true);
	DrawBox(631, 0, 900, 506, GetColor(100, 100, 100), true);
	DrawBox(0, 0, 900, 70, GetColor(100, 100, 100), true);
	DrawBox(0, 470, 900, 506, GetColor(100, 100, 100), true);*/
	DrawBoxAA(20, 20, 280, 490, GetColor(255, 255, 255), false, 4);
}

void UI::DrawScore(int score)
{
	if (isRendition == true) {
		DrawScoreRend();
	}
	DrawFormatStringToHandle(30, 120, whiteColor, fontData[1], "score : %.8d", score);

}

void UI::DrawTime(int spawnRemain)
{
	DrawFormatStringToHandle(30, 144, whiteColor, fontData[1], "spawnRemain : %u", spawnRemain);
	//DrawFormatString(0, 128, whiteColor, "generate rate : %u", spawnTime / 60u);
}

void UI::DrawLevel(int level)
{
	DrawFormatStringToHandle(30, 168, whiteColor, fontData[1], "level : %u", level);
	if (isLevelUp == true) {
		DrawLevUpRend();
	}
}

void UI::DrawScoreRend()
{
	if (isScoRend) {
		if (scoreRenditionTime % 2) {
			DrawFormatStringToHandle(130, 96 - scoreRenditionTime, whiteColor, fontData[1], "+%d", addScore);
		}
		else
		{
			DrawFormatStringToHandle(130, 96 - scoreRenditionTime, renditionColor, fontData[1], "+%d", addScore);
		}
	}

}

void UI::DrawLevUpRend()
{
	if (isLevRend) {
		if (levelUpRenditionTime % 2) {
			DrawFormatStringToHandle(130, 192 + levelUpRenditionTime, whiteColor, fontData[1], "LEVEL UP!");
		}
		else {
			DrawFormatStringToHandle(130, 192 + levelUpRenditionTime, renditionColor, fontData[1], "LEVEL UP!");
		}
	}
}

void UI::DrawGameOver()
{
	DrawFormatStringToHandle(0, 80, whiteColor, fontData[2], "GAME OVER");
}

void UI::DrawChangeIn()
{
	DrawBox(900 - (changeSceneTime * 15), 0, 1800 - (changeSceneTime * 30), 506, GetColor(100, 100, 100), true);
	DrawBox(900, 0, 1800 - (changeSceneTime * 30), 506, GetColor(0, 0, 0), true);
}

void UI::DrawChangeOut()
{
	DrawBox(0, 0, 900 - (changeSceneTime * 15), 506, GetColor(100, 100, 100), true);
	DrawBox(0, 0, 900 - (changeSceneTime * 30), 506, GetColor(0, 0, 0), true);
}

void UI::AddScoreRendition()
{
	scoreRenditionTime++;
	if (scoreRenditionTime > 30) {
		if (isScoRend){
			isScoRend = false;
			isRendition = false;
		}
		scoreRenditionTime = 0;
	}
}

void UI::LevelUpRendition()
{
	levelUpRenditionTime++;
	if (levelUpRenditionTime > 30) {
		if (isLevRend) {
			isLevRend = false;
			isLevelUp = false;
		}
		levelUpRenditionTime = 0;
	}
}

void UI::ChangeSceneIn(ISceneChanger* mSceneChanger)
{
	if (changeSceneTime < 60) {
		changeSceneTime++;
	}
	if (changeSceneTime >= 60) {
		mSceneChanger->ChangeScene(eScene_Game);
	}
}

void UI::ChangeSceneOut()
{
}

void UI::CreateFontData()
{
	fontData[0] = CreateFontToHandle(NULL, -1, -1);
	fontData[1] = CreateFontToHandle("03SmartFontUI", 24, -1, DX_FONTTYPE_ANTIALIASING);
	fontData[2] = CreateFontToHandle("03SmartFontUI", 64, -1, DX_FONTTYPE_ANTIALIASING);
}

void UI::CreateResourceData()
{
	LoadDivGraph("Resources/pieces.png", 8, 8, 1, 32, 32, pieceData,false);

}

void UI::Finalize()
{
	for (int i = 0; i < fontQuantity; i++) {
		RemoveFontResourceEx("Resource/FontData/03SmartFontUI.otf", FR_PRIVATE, NULL);
		DeleteFontToHandle(fontData[i]);
	}
}

void UI::DebugDraw(int mouseX, int mouseY)
{
	//DrawFormatStringToHandle(0, 0, whiteColor, fontData[1], "x %d  y %d", mouseX, mouseY);
	//DrawFormatStringToHandle(0, 24, whiteColor, fontData[1], "WASD :   box move");
	//DrawFormatStringToHandle(0, 48, whiteColor, fontData[1], "space :  piece rotate (�����v���)");
	//DrawFormatStringToHandle(0, 72, whiteColor, fontData[1], "U :  piece generate");
	//DrawFormatStringToHandle(0, 96, whiteColor, fontData[1], "R :  reset");
	//DrawFormatStringToHandle(0, 0, whiteColor, "x %d  y %d", mouseX, mouseY);
	/*DrawFormatString(0, 16, whiteColor, "WASD :   box move");
	DrawFormatString(0, 32, whiteColor, "space :  piece rotate (�����v���)");
	DrawFormatString(0, 48, whiteColor, "U :  piece generate");
	DrawFormatString(0, 64, whiteColor, "R :  reset");*/
}
