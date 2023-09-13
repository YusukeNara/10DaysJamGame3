#include "ScoreManager.h"

#include "Input.h"

using namespace Rv3Ease;

ScoreManager::ScoreManager()
{
	bigfontHandle = CreateFontToHandle("03スマートフォントUI", 48, 9, DX_FONTTYPE_ANTIALIASING_16X16);
	smallFontHandle = CreateFontToHandle("03スマートフォントUI", 24, 9, DX_FONTTYPE_ANTIALIASING_16X16);
}

void ScoreManager::Init()
{
	isDisplayScore = false;

	gameoverTextEase.Init(RV3_EASE_TYPE::EASE_CUBIC_OUT, RVector3(300, -200, 0), RVector3(300, 100, 0), 60);
	scoreEase.Init(RV3_EASE_TYPE::EASE_CUBIC_OUT, RVector3(325, -200, 0), RVector3(325, 300, 0), 60);
	gameoverTextEase.Reset();
	scoreEase.Reset();

	frame = 0;
}

void ScoreManager::Update()
{
	if (!isDisplayScore) { return; }

	if (isDisplayScore) {
		frame++;
	}

	if (frame == 60) {
		gameoverTextEase.Play();
	}
	if (frame == 120) {
		scoreEase.Play();
	}

	gameoverTextEase.Update();
	scoreEase.Update();

	//キー入力でスコア表示終了
	if (Input::IsKeyTrigger(KEY_INPUT_R)) {
		Init();
	}

}

void ScoreManager::Draw()
{
	if (!isDisplayScore) { return; }

	DrawStringToHandle(gameoverTextEase.GetNowpos().x, gameoverTextEase.GetNowpos().y, "GAMEOVER", GetColor(255, 255, 255), bigfontHandle);
	DrawFormatStringToHandle(scoreEase.GetNowpos().x, scoreEase.GetNowpos().y, GetColor(255, 255, 255), smallFontHandle,		"NEW  SCORE   %d", newScore);
	DrawFormatStringToHandle(scoreEase.GetNowpos().x, scoreEase.GetNowpos().y + 50, GetColor(255, 255, 255), smallFontHandle,	"HIGH SCORE   %d", highScore);
	DrawFormatStringToHandle(scoreEase.GetNowpos().x, scoreEase.GetNowpos().y + 100, GetColor(255, 255, 255), smallFontHandle, "Press R to Continue", highScore);
}

void ScoreManager::StartScoreDraw(int newScore, int level)
{
	if (isDisplayScore) { return; }

	this->newScore = newScore;
	this->level = level;

	if (highScore < newScore) {
		highScore = newScore;
	}

	frame = 0;

	isDisplayScore = true;
}
