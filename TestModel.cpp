#include "TestModel.h"
#include"Engine/Input.h"
#include "Engine/Model.h"

//�R���X�g���N�^
TestModel::TestModel(GameObject* parent)
    :GameObject(parent, "TestModel"), hModel_(-1)
{
    
}

//�f�X�g���N�^
TestModel::~TestModel()
{
}

//������
void TestModel::Initialize()
{
    transform_.position_.x = (float)(rand() % 30 - 15) / 10;
    transform_.position_.z = +15;
    transform_.scale_.x = 0.5;
    transform_.scale_.y = 0.5;
    transform_.scale_.z = 0.5;
    //���f���f�[�^�̃��[�h
    /*hModel_ = Model::Load("Assets\\oden.fbx");
    assert(hModel_ >= 0);*/
    
    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 0.8f);
    AddCollider(collision);
    
}

//�X�V
void TestModel::Update()
{
    
    //�G�̓���
    transform_.position_.z -= 0.1;
    if (transform_.position_.z <= -15)
    {
        KillMe();
    }


}

//�`��
void TestModel::Draw()
{

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void TestModel::Release()
{

}

void TestModel::OnCollision(GameObject* pTarget)
{
    //�e�ɓ��������Ƃ�
    if (pTarget->GetObjectName() == "Ballet")
    {
        KillMe();
        pTarget->KillMe();
    }
}
