#include "FPSControl.h"
#include <DxLib.h>

void FPSControl::FpsControll_Initialize()
{
	mStartTime = GetNowCount();
	mCount = 0;
	mFps = 0;
}

bool FPSControl::FpsControll_Update()
{
    if (mCount == 0) { //1フレーム目なら時刻を記憶
        mStartTime = GetNowCount();
    }
    if (mCount == N) { //60フレーム目なら平均を計算する
        int t = GetNowCount();
        mFps = 1000.f / ((t - mStartTime) / (float)N);
        mCount = 0;
        mStartTime = t;
    }
    mCount++;
    return true;
}

void FPSControl::FpsControll_Draw()
{
    //DrawFormatString(700, 0, GetColor(255, 255, 255), "%.1f", mFps);
}

void FPSControl::FpsControll_Wait()
{
    int tookTime = GetNowCount() - mStartTime;  //かかった時間
    int waitTime = mCount * 1000 / FPS - tookTime;  //待つべき時間
    if (waitTime > 0) {
        Sleep(waitTime);  //待機
    }
}