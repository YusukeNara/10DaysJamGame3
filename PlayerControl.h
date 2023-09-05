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


	//ボードにアクセスするポインタ
	Board* board;

};

