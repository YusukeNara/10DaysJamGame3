#pragma once

#include "BaseScene.h"
#include "ISceneChanger.h"

#include "Board.h"
#include "PlayerControl.h"
#include "UI.h"


//�Q�[����ʃN���X
class Game : public BaseScene {

public:
    Game(ISceneChanger *changer);
    void Initialize() override;    //�������������I�[�o�[���C�h�B
    //void Finalize() override;        //�I���������I�[�o�[���C�h�B
    void Update() override;        //�X�V�������I�[�o�[���C�h�B
    void Draw() override;            //�`�揈�����I�[�o�[���C�h�B

    void flagInit();

    bool changeOut;
    bool changeMenu;

    //-----�ȉ��A�Q�[�����Ŏg�p����C���X�^���X-----//
    Board board;

    PlayerControl player;

    UI ui;

};

