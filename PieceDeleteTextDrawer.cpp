#include "PieceDeleteTextDrawer.h"
#include <DxLib.h>

#include "Board.h"

using namespace Rv3Ease;

void PieceDeleteTextDrawer::ComboText::Init(int sx, int sy, int handle, int frame, int comboNum)
{
	this->comboNum = comboNum;
	fontHandle = handle;
	fontEase.Init(RV3_EASE_TYPE::EASE_CUBIC_OUT,
		RVector3(sx, sy, 0),
		RVector3(sx, sy - 50, 0),
		frame);
	fontEase.Reset();
	fontEase.Play();
}

void PieceDeleteTextDrawer::ComboText::Update()
{
	fontEase.Update();
}

void PieceDeleteTextDrawer::ComboText::Draw()
{
	DrawFormatStringToHandle(fontEase.GetNowpos().x, fontEase.GetNowpos().y,
		GetColor(255, 255, 255), fontHandle, "%d combo!", comboNum);
}

void PieceDeleteTextDrawer::Init()
{
	comboTextHandle = CreateFontToHandle("03スマートフォントUI", 16, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
	specialTextHandle = CreateFontToHandle("03スマートフォントUI", 32, -1, DX_FONTTYPE_ANTIALIASING_EDGE_16X16);

	specialTextEase.Init(Rv3Ease::RV3_EASE_TYPE::EASE_CUBIC_OUT,
		RVector3(400, 300, 0),
		RVector3(400, 200, 0),
		30);
}

void PieceDeleteTextDrawer::Update()
{
	std::erase_if(comboTexts, [](PieceDeleteTextDrawer::ComboText p) {return p.IsEnd(); });

	for (auto& texts : comboTexts) {
		texts.Update();
	}
	specialTextEase.Update();

	if (specialTextEase.IsEnd()) {
		specialTextEase.Reset();
	}
}

void PieceDeleteTextDrawer::Draw()
{
	for (auto& texts : comboTexts) {
		texts.Draw();
	}

	if (specialTextEase.IsPlay() && !specialTextEase.IsEnd()) {
		DrawStringToHandle(specialTextEase.GetNowpos().x,
			specialTextEase.GetNowpos().y,
			specialText.c_str(),
			GetColor(255, 255, 55),
			specialTextHandle,
			GetColor(155, 155, 155));
	}
}

void PieceDeleteTextDrawer::DrawComboText(int x, int y, int frame, int comboNum)
{
	ComboText newCombo;
	newCombo.Init(x, y, comboTextHandle, frame, comboNum);
	comboTexts.push_back(newCombo);
}

void PieceDeleteTextDrawer::DrawSpecialText(int specialNum)
{
	if (specialNum == 0) {
		specialText = "STRAIGHT!";
	}
	if (specialNum == 1) {
		specialText = "DIAMOND!";
	}
	if (specialNum == 2) {
		specialText = "VICTORY!";
	}
	specialTextEase.Reset();
	specialTextEase.Play();
}
