#pragma once
class FPSControl
{
public:
	void FpsControll_Initialize();
	bool FpsControll_Update();
	void FpsControll_Draw();
	void FpsControll_Wait();

private:
	int mStartTime;      //����J�n����
	int mCount;          //�J�E���^
	float mFps;          //fps
	const int N = 60;  //���ς����T���v����
	const int FPS = 60;  //�ݒ肵��FPS
};

