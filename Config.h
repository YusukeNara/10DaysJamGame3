#pragma once
#pragma once

#include "BaseScene.h"
#include <DxLib.h>
#include <array>

//設定画面クラス
class Config : public BaseScene {

public:
    Config(ISceneChanger *changer);
    void Initialize() override;    //初期化処理をオーバーライド。
    //void Finalize() override;        //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;            //描画処理をオーバーライド。

    //チュートリアルに使用するリソース
    int textHandle = 0;
    //ピース画像
    std::array<int, 8> pieceHandle;
    //ページ
    int page = 0;
    //画像
    std::array<int, 3> pages;

};

