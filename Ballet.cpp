#include "Ballet.h"
#include"Engine/Input.h"
#include "Engine/Model.h"
//�R���X�g���N�^
Ballet::Ballet(GameObject* parent)
    :GameObject(parent, "Ballet"), hModel_(-1)
{
}

//�f�X�g���N�^
Ballet::~Ballet()
{
}

//������
void Ballet::Initialize()
{
    transform_.scale_.x = 0.3;
    transform_.scale_.y = 0.3;
    transform_.scale_.z = 0.3;
    
    ////���f���f�[�^�̃��[�h
    //hModel_ = Model::Load("Assets\\oden.fbx");
    //assert(hModel_ >= 0);

    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
    AddCollider(collision);
}

//�X�V
void Ballet::Update()
{
    //�e�̓���
    transform_.position_.z += 0.2;
    if ( transform_.position_.z >= 20 )
    {
        KillMe();
    }
}

//�`��
void Ballet::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}


void Ballet::Release()
{
}