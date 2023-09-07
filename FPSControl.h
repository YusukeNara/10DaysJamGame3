#pragma once
class FPSControl
{
public:
	void FpsControll_Initialize();
	bool FpsControll_Update();
	void FpsControll_Draw();
	void FpsControll_Wait();

private:
	int mStartTime;      //測定開始時刻
	int mCount;          //カウンタ
	float mFps;          //fps
	const int N = 60;  //平均を取るサンプル数
	const int FPS = 60;  //設定したFPS
};

