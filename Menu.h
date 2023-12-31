#pragma once

#include "BaseScene.h"
#include "UI.h"

//メニュー画面クラス
class Menu : public BaseScene {

public:
    Menu(ISceneChanger *changer);
    void Initialize() override;    //初期化処理をオーバーライド。
    //void Finalize() override ;        //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;            //描画処理をオーバーライド。


private:
    int renditionTime;
    int rotationTime;

    int piece[8];

    bool sceneChange;
private:
    UI ui;

    //フォントハンドル
    static const int fontQuantity = 1;
    float PI = 3.14159;
    int fontHandle[fontQuantity];
};

