#pragma once
#include <DxLib.h>

#include "Board.h"
#include "Input.h"

class PlayerControl
{
public:

	void Init(Board* board);

	void UserControl();

	void Draw();

private:
	int selectX = 0;
	int selectY = 0;

	const int SELECTER_WIDTH = 2;
	const int SELECTER_LENGHT = 10;

	int handle = 0;

	//ボードにアクセスするポインタ
	Board* board;

};

