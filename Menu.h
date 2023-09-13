#pragma once

#include "BaseScene.h"
#include "UI.h"

//���j���[��ʃN���X
class Menu : public BaseScene {

public:
    Menu(ISceneChanger *changer);
    void Initialize() override;    //�������������I�[�o�[���C�h�B
    //void Finalize() override ;        //�I���������I�[�o�[���C�h�B
    void Update() override;        //�X�V�������I�[�o�[���C�h�B
    void Draw() override;            //�`�揈�����I�[�o�[���C�h�B


private:
    int renditionTime;
    int rotationTime;

    int piece[8];

    bool sceneChange;
private:
    UI ui;

    //�t�H���g�n���h��
    static const int fontQuantity = 1;
    float PI = 3.14159;
    int fontHandle[fontQuantity];
};

