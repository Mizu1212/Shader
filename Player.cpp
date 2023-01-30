#include "Player.h"
#include"Engine/Input.h"
#include "Ballet.h"
#include "Engine/Model.h"
#include"MiniOden.h"

//�R���X�g���N�^
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"), hModel_(-1)
{
}

//�f�X�g���N�^
Player::~Player()
{
}

//������
void Player::Initialize()
{
    
    transform_.position_.y = -2;
    transform_.scale_.x = 2;
    transform_.scale_.y = 2;
    transform_.scale_.z = 2;
    
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets\\water.fbx",SHADER_WATER);
    assert(hModel_ >= 0);

    //Instantiate<MiniOden>(this);
}

//�X�V
void Player::Update()
{
    //transform_.rotate_.y++;
   /* if (Input::IsKey(DIK_A))
    {
        transform_.position_.x -= 0.1f;
    }
    if (Input::IsKey(DIK_D))
    {
        transform_.position_.x += 0.1f;
    }
    if (Input::IsKeyDown(DIK_SPACE))
    {
        
        pBallet = PInstantiate<Ballet>(GetParent());
        pBallet->SetPosition(transform_.position_);
    }*/
}

//�`��
void Player::Draw()
{
    
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Player::Release()
{
   
}