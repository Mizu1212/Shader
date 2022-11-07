#include "SceneManager.h"
#include "../PlayScene.h"
#include"../GameOver.h"
#include"../GameStartScene.h"
#include "Model.h"

//�R���X�g���N�^
SceneManager::SceneManager(GameObject* parent)
	: GameObject(parent, "SceneManager")
{
}
//������
void SceneManager::Initialize()
{
	//�ŏ��̃V�[��������
	firstSceneID_ = SCENE_ID_GameStart;
	nextSceneID_ = firstSceneID_;
	Instantiate<GameStartScene>(this);
}
//�X�V
void SceneManager::Update()
{
	//���̃V�[�������݂̃V�[���ƈႤ�@���@�V�[����؂�ւ��Ȃ���΂Ȃ�Ȃ�
	if (firstSceneID_ != nextSceneID_)
	{
		//���̃V�[���̃I�u�W�F�N�g��S�폜
		KillAllChildren();
		//���[�h�����f�[�^��S�폜
		Model::AllRelease();

		//���̃V�[�����쐬
		switch (nextSceneID_)
		{
		case SCENE_ID_PlayScene: Instantiate<PlayScene>(this); break;
		case SCENE_ID_GameOver: Instantiate<GameOver>(this); break;
		case SCENE_ID_GameStart: Instantiate<GameStartScene>(this); break;
		}
		firstSceneID_ = nextSceneID_;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}