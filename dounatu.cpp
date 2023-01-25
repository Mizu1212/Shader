#include "dounatu.h"
#include"Engine/Input.h"
#include "Ballet.h"
#include "Engine/Model.h"
#include"MiniOden.h"

//�R���X�g���N�^
dounatu::dounatu(GameObject* parent)
    :GameObject(parent, "dounatu"), hModel_(-1)
{
}

//�f�X�g���N�^
dounatu::~dounatu()
{
}

//������
void dounatu::Initialize()
{

    //transform_.position_.y = -2;
    transform_.scale_.x = 0.7;
    transform_.scale_.y = 0.7;
    transform_.scale_.z = 0.7;

    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets\\Torus2.fbx");
    assert(hModel_ >= 0);
    //Instantiate<MiniOden>(this);
}

//�X�V
void dounatu::Update()
{
    //transform_.rotate_.y++;
    if (Input::IsKey(DIK_UP))
    {
        transform_.position_.y += 0.1f;
    }
    if (Input::IsKey(DIK_DOWN))
    {
        transform_.position_.y -= 0.1f;
    }
    
}

//�`��
void dounatu::Draw()
{

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void dounatu::Release()
{

}