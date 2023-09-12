#pragma once
#include "RVector.h"

#include <string>
#include <vector>

class PieceDeleteTextDrawer
{
private:
	//ƒRƒ“ƒ{•\Ž¦
	class ComboText
	{
	public:
		ComboText() = default;
		~ComboText() = default;

		void Init(int sx, int sy, int handle, int frame, int comboNum);

		void Update();

		void Draw();

		bool IsEnd() { return fontEase.IsEnd(); };

	private:
		int fontHandle = 0;
		int comboNum = 0;
		Rv3Ease::Rv3Easing fontEase;
	};

public:
	PieceDeleteTextDrawer() = default;
	~PieceDeleteTextDrawer() = default;

	void Init();

	void Update();

	void Draw();

	void DrawComboText(int x, int y, int frame, int comboNum);

	void DrawSpecialText(int specialNum);

private:
	std::vector<ComboText> comboTexts;

	int comboTextHandle = 0;
	int specialTextHandle = 0;

	std::string specialText;

	Rv3Ease::Rv3Easing specialTextEase;

};

