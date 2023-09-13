#pragma once
#pragma once

#include "BaseScene.h"
#include <DxLib.h>
#include <array>

//�ݒ��ʃN���X
class Config : public BaseScene {

public:
    Config(ISceneChanger *changer);
    void Initialize() override;    //�������������I�[�o�[���C�h�B
    //void Finalize() override;        //�I���������I�[�o�[���C�h�B
    void Update() override;        //�X�V�������I�[�o�[���C�h�B
    void Draw() override;            //�`�揈�����I�[�o�[���C�h�B

    //�`���[�g���A���Ɏg�p���郊�\�[�X
    int textHandle = 0;
    //�s�[�X�摜
    std::array<int, 8> pieceHandle;
    //�y�[�W
    int page = 0;
    //�摜
    std::array<int, 3> pages;

};

