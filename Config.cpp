#include "Config.h"
#include "DxLib.h"

#include "Input.h"

Config::Config(ISceneChanger *changer) : BaseScene(changer) {
}

//������
void Config::Initialize() {
    
    for (auto& p : pages) {
        p = MakeScreen(900, 506, TRUE);
    }

    LoadDivGraph("Resources/pieces.png", 8, 8, 1, 32, 32, pieceHandle.data());
    textHandle = CreateFontToHandle("03�X�}�[�g�t�H���gUI", 32, 9, DX_FONTTYPE_ANTIALIASING_16X16);

    SetDrawScreen(pages[0]);

    int color = GetColor(255, 255, 255);

    DrawStringToHandle(16, 16, "������@", color, textHandle);


    DrawStringToHandle(32, 400, "WASD �ړ�", color, textHandle);
    DrawStringToHandle(32, 432, "U �s�[�X�㏸", color, textHandle);
    DrawStringToHandle(370, 400, "SPACE �s�[�X��]", color, textHandle);
    DrawStringToHandle(370, 432, "SPACE �Ŏ���", color, textHandle);

    SetDrawScreen(pages[1]);

    for (int x = 0; x < 3; x++) {
        DrawExtendGraph(100 + 64 * x, 250, 100 + 64 * x + 64, 312, pieceHandle[0], TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawExtendGraph(450 + 64 * x, 250, 450 + 64 * x + 64, 312, pieceHandle[0], TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
    }

    DrawStringToHandle(32, 368, "�����F��3���낦�ď������I", color, textHandle);
    DrawStringToHandle(32, 400, "�������A��x�������F�͑��̐F�������܂ŏ����Ȃ����I", color, textHandle);

    SetDrawScreen(pages[2]);

    for (int x = 0; x < 5; x++) {
        DrawExtendGraph(100 + 64 * x, 100, 100 + 64 * x + 64, 164, pieceHandle[1], TRUE);
    }

    DrawExtendGraph(632,        168,        664,        200, pieceHandle[3], TRUE);
    DrawExtendGraph(632 - 32,   168 + 32,   664 - 32,   200 + 32, pieceHandle[3], TRUE);
    DrawExtendGraph(632 - 32,   168 + 64,   664 - 32,   200 + 64, pieceHandle[3], TRUE);
    DrawExtendGraph(632,        168 + 96,   664,        200 + 96, pieceHandle[3], TRUE);
    DrawExtendGraph(632 + 32,   168 + 32,   664 + 32,   200 + 32, pieceHandle[3], TRUE);
    DrawExtendGraph(632 + 32,   168 + 64,   664 + 32,   200 + 64, pieceHandle[3], TRUE);

    for (int x = 1; x < 3; x++) {
        DrawExtendGraph(400 - 32 * x, 300 - 32 * x, 432 - 32 * x, 332 - 32 * x, pieceHandle[6],TRUE);
        DrawExtendGraph(400 + 32 * x, 300 - 32 * x, 432 + 32 * x, 332 - 32 * x, pieceHandle[6], TRUE);
    }
    DrawExtendGraph(400, 300, 432, 332, pieceHandle[6], TRUE);

    DrawStringToHandle(32, 368, "����ȕ��ו������邱�ƂŃ{�[�i�X�����I", color, textHandle);
    DrawStringToHandle(32, 400, "���������������A�Ăя�����悤�ɂȂ����肷�邼�I", color, textHandle);
    DrawStringToHandle(32, 432, "SPACE�Ń^�C�g���ɖ߂�", color, textHandle);

    SetDrawScreen(DX_SCREEN_BACK);

}

//�X�V
void Config::Update() {

    if (Input::IsKeyTrigger(KEY_INPUT_SPACE) && page == 2) {
        mSceneChanger->ChangeScene(eScene::eScene_Menu);
    }

    if (Input::IsKeyTrigger(KEY_INPUT_SPACE) && page < 2) {
        page++;
    }


}

//�`��
void Config::Draw() {
    
    DrawGraph(0, 0, pages[page], TRUE);
}