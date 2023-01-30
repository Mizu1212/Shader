#include "MiniOden.h"
#include "Engine/Model.h"

//�R���X�g���N�^
MiniOden::MiniOden(GameObject* parent)
    :GameObject(parent,"MiniOden"), hModel_(-1)
{
}

//�f�X�g���N�^
MiniOden::~MiniOden()
{
}

//������
void MiniOden::Initialize()
{
    transform_.position_.x = +1;
    transform_.position_.z = -1;
    transform_.scale_.x = 0.3;
    transform_.scale_.y = 0.3;
    transform_.scale_.z = 0.3;
    //���f���f�[�^�̃��[�h
   /* hModel_ = Model::Load("Assets\\oden.fbx");
    assert(hModel_ >= 0);*/
}

//�X�V
void MiniOden::Update()
{
}

//�`��
void MiniOden::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void MiniOden::Release()
{
}