#pragma once

class Input final
{
public:
	static char keys[256];
	static char oldkeys[256];

private:
	// コンストラクタとデストラクタをprivateに
	Input() {};
	~Input() {};

public:
	Input(const Input& obj) = delete; //コピーコンストラクタの無効化
	Input& operator=(const Input& obj) = delete; //代入演算子の無効化

	// 入力の更新
	static void Update();

	// キーを押しているかどうかの判定
	static bool IsKey(int keyCode);
	// キーを押した瞬間かどうかの判定
	static bool IsKeyTrigger(int keyCode);
	// キーを離した瞬間かどうかの判定
	static bool IsKeyReturn(int keyCode);
};
