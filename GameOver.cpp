#include "GameOver.h"
#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Image.h"


//�R���X�g���N�^
GameOver::GameOver(GameObject* parent)
	: GameObject(parent, "GameOver"), hPict_(-1)
{
}

//������
void GameOver::Initialize()
{
	//�摜�f�[�^�̃��[�h
	hPict_ = Image::Load("Assets\\Kinniku.jpg");
	assert(hPict_ >= 0);
}

//�X�V
void GameOver::Update()
{
	
}

//�`��
void GameOver::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

//�J��
void GameOver::Release()
{
}