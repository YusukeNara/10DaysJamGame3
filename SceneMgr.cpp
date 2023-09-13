#include "DxLib.h"
#include "Config.h"
#include "Game.h"
#include "Menu.h"
#include "SceneMgr.h"

SceneMgr::SceneMgr() :
    mNextScene(eScene_None) //���̃V�[���Ǘ��ϐ�
{
    mScene = (BaseScene *) new Menu(this);
}

//������
void SceneMgr::Initialize() {
    mScene->Initialize();
}

//�I������
void SceneMgr::Finalize() {
    mScene->Finalize();
}

//�X�V
void SceneMgr::Update() {
    if (mNextScene != eScene_None) {    //���̃V�[�����Z�b�g����Ă�����
        mScene->Finalize();//���݂̃V�[���̏I�����������s
        switch (mNextScene) {       //�V�[���ɂ���ď����𕪊�
        case eScene_Menu:        //���̉�ʂ����j���[�Ȃ�
            mScene = (BaseScene*) new Menu(this);   //���j���[��ʂ̃C���X�^���X�𐶐�����
            break;//�ȉ���
        case eScene_Game:
            mScene = (BaseScene*) new Game(this);
            break;
        case eScene_Config:
            mScene = (BaseScene*) new Config(this);
            break;
        }
        mScene->Initialize();    //�V�[����������
        mNextScene = eScene_None;    //���̃V�[�������N���A
    }


    mScene->Update(); //�V�[���̍X�V
    //�V�[���J�ډ��o�X�V

}

//�`��
void SceneMgr::Draw() {
    mScene->Draw(); //�V�[���̕`��
}

// ���� nextScene �ɃV�[����ύX����
void SceneMgr::ChangeScene(eScene NextScene) {
    mNextScene = NextScene;    //���̃V�[�����Z�b�g����
}
