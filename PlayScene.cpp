#include "PlayScene.h"
#include"Player.h"
#include"MiniOden.h"
#include "Engine/SceneManager.h"
#include"Engine/Input.h"
#include"TestModel.h"

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), count(0), Ecount(0)
{
}

//������
void PlayScene::Initialize()
{
	
	Instantiate<Player>(this);
	Instantiate<TestModel>(this);
	
}

//�X�V
void PlayScene::Update()
{
	if (count >= 60)
	{
		Instantiate<TestModel>(this);
		count = 0;
		Ecount++;
	}
	if (Input::IsKeyDown(DIK_P))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GameOver);
	}
	if (Ecount >= 10)
	{
		if (FindObject("TestModel") == nullptr)
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_GameOver);
		}
	}
	
	count++;
}

//�`��
void PlayScene::Draw()
{
	
}

//�J��
void PlayScene::Release()
{
}