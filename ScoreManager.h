#pragma once
#include <DxLib.h>

#include "RVector.h"

class ScoreManager
{
public:
	ScoreManager();
	~ScoreManager() = default;

	void Init();

	void Update();

	void Draw();

	void StartScoreDraw(int newScore, int level);


private:
	int bigfontHandle = 0;
	int smallFontHandle = 0;

	bool isDisplayScore = false;

	int frame = 0;

	int highScore = 0;
	int newScore = 0;
	int level = 0;

	Rv3Ease::Rv3Easing gameoverTextEase;
	Rv3Ease::Rv3Easing scoreEase;

};