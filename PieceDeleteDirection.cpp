#include "PieceDeleteDirection.h"

#include "NY_random.h"

void PieceDeleteDirection::PieceDeleteDirectionEmiter::Init(RVector3 start, RVector3 end, int graphHandle, int frame)
{
	RVector3 power = RVector3(
		NY_random::floatrand_sl(start.x + 500, start.x - 500),
		NY_random::floatrand_sl(start.y + 500, start.y - 500),
		0);
	bezierEase.Init(start, end, power, frame, Rv3Ease::RV3_EASE_TYPE::EASE_QUAD_IN);
	this->graphHandle = graphHandle;
	bezierEase.Reset();
	bezierEase.Play();

	size = NY_random::intrand_sl(16, 4);
}

void PieceDeleteDirection::PieceDeleteDirectionEmiter::Update()
{
	bezierEase.Update();
}

void PieceDeleteDirection::PieceDeleteDirectionEmiter::Draw()
{
	DrawCircle(bezierEase.nowPos().x,
		bezierEase.nowPos().y,
		size,
		GetColor(255, 255, 255));
}

void PieceDeleteDirection::Init()
{
}

void PieceDeleteDirection::Update()
{
	//演出終了パーティクルを消去
	std::erase_if(emiter, [](PieceDeleteDirectionEmiter p) {return p.isEnd(); });

	for (auto& e : emiter) {
		e.Update();
	}
}

void PieceDeleteDirection::Draw()
{
	for (auto& e : emiter) {
		e.Draw();
	}
}

void PieceDeleteDirection::PlayEase(int sx, int sy, int ex, int ey, int frame,int generateNum)
{
	for (int x = 0; x < generateNum; x++) {
		PieceDeleteDirectionEmiter newEmiter;
		newEmiter.Init(RVector3(sx, sy, 0),
			RVector3(ex, ey, 0),
			0,
			frame);

		emiter.push_back(newEmiter);
	}
}
