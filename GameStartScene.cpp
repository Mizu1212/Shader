#include "GameStartScene.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include"Engine/Input.h"
//�R���X�g���N�^
GameStartScene::GameStartScene(GameObject* parent)
	: GameObject(parent, "GameStartScene"), hPict_(-1)
{
}

//������
void GameStartScene::Initialize()
{
	//�摜�f�[�^�̃��[�h
	hPict_ = Image::Load("Assets\\GameStart.png");
	assert(hPict_ >= 0);
}

//�X�V
void GameStartScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PlayScene);
	}
}

//�`��
void GameStartScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

//�J��
void GameStartScene::Release()
{
}