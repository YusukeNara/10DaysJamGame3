#pragma once
#include <DxLib.h>
#include "BaseScene.h"

class UI
{
public:
	UI();
	~UI();

	void Init();
	void Update();
	void AddScore(int addScore);
	void LevelUp();
	
	void DrawFlame();
	void DrawScore(int score);
	void DrawTime(int spawnRemain);
	void DrawLevel(int level);
	void DrawScoreRend();
	void DrawLevUpRend();
	void DrawGameOver();
	void DrawChangeIn();
	void DrawChangeOut();

	void AddScoreRendition();
	void LevelUpRendition();

	void ChangeSceneIn(ISceneChanger* mSceneChanger);
	void ChangeSceneOut();

	//init
	void CreateFontData();
	void CreateResourceData();
	//fin
	void Finalize();

	//debug
	void DebugDraw(int mouseX,int mouseY);
private:
	int addScore;
	int time;
	int scoreRenditionTime;
	int levelUpRenditionTime;
	int changeSceneTime;
	static const int fontQuantity = 3;
	static const int pieceCount = 8;

	int renditionColor;
	int whiteColor;
	
	bool isLevelUp;
	bool isRendition;
	bool isScoRend;
	bool isLevRend;

	int fontData[fontQuantity];
	int pieceData[pieceCount];
};

