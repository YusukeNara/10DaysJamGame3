#include "PlayerControl.h"

#include "PieceData.h"

void PlayerControl::Init(Board* board)
{
	this->board = board;

	handle = LoadGraph("Resources/selecter.png");
}

void PlayerControl::UserControl()
{
	if (Input::IsKeyTrigger(KEY_INPUT_W) && selectY + 2 < Board::BOARD_HEIGHT) {
		selectY++;
	}
	if (Input::IsKeyTrigger(KEY_INPUT_S) && selectY > 0) {
		selectY--;
	}
	if (Input::IsKeyTrigger(KEY_INPUT_D) && selectX + 2 < Board::BOARD_WIDTH) {
		selectX++;
	}
	if (Input::IsKeyTrigger(KEY_INPUT_A) && selectX > 0) {
		selectX--;
	}

	if (Input::IsKeyTrigger(KEY_INPUT_SPACE)) {
		board->StartPieceRotate(selectX, selectY);
	}

}

void PlayerControl::Draw()
{
	//ï`âÊç¿ïW

	float PI = 3.14f;

	//for (int y = 0; y < 2; y++) {
	//	float rotY = PI / (180 * y);
	//	for (int x = 0; x < 2; x++) {
	//		float rotX = PI / (180 * x);

	//		//
	//		int centerX = PieceData::DRAWBASE_X + (selectX + x) * (PieceData::PIECE_SIZE);
	//		int centerY = PieceData::DRAWBASE_Y - (selectY + y) * (PieceData::PIECE_SIZE);
	//		
	//		DrawRotaGraph2(centerX, centerY, 40, 0, 1.0, rotX + rotY, handle, true);
	//	}
	//}

	int lx = PieceData::DRAWBASE_X + selectX * (PieceData::PIECE_SIZE)-PieceData::PIECE_SIZE / 2;
	int rx = PieceData::DRAWBASE_X + (selectX + 1) * (PieceData::PIECE_SIZE)+PieceData::PIECE_SIZE / 2;
	int ty = PieceData::DRAWBASE_Y - selectY * (PieceData::PIECE_SIZE)+PieceData::PIECE_SIZE / 2;
	int by = PieceData::DRAWBASE_Y - (selectY + 1) * (PieceData::PIECE_SIZE)-PieceData::PIECE_SIZE / 2;

	DrawExtendGraph(lx, ty, rx, by, handle, true);


}
