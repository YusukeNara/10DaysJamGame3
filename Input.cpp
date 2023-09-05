#include "Input.h"
#include <DxLib.h>

char Input::keys[256] = { 0 };
char Input::oldkeys[256] = { 0 };

void Input::Update()
{
	for (int i = 0; i < 256; i++)
	{
		oldkeys[i] = keys[i];
	}
	GetHitKeyStateAll(keys);
}

bool Input::IsKey(int keyCode)
{
	return keys[keyCode];
}

bool Input::IsKeyTrigger(int keyCode)
{
	return keys[keyCode] && !oldkeys[keyCode];
}

bool Input::IsKeyReturn(int keyCode)
{
	return !keys[keyCode] && oldkeys[keyCode];
}
