#pragma once

#include "BaseScene.h"
#include "ISceneChanger.h"

#include "Board.h"
#include "PlayerControl.h"
#include "UI.h"


//ゲーム画面クラス
class Game : public BaseScene {

public:
    Game(ISceneChanger *changer);
    void Initialize() override;    //初期化処理をオーバーライド。
    //void Finalize() override;        //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;            //描画処理をオーバーライド。

    void flagInit();

    bool changeOut;
    bool changeMenu;

    //-----以下、ゲーム内で使用するインスタンス-----//
    Board board;

    PlayerControl player;

    UI ui;

};

