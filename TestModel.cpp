#include "TestModel.h"
#include"Engine/Input.h"
#include "Engine/Model.h"

//コンストラクタ
TestModel::TestModel(GameObject* parent)
    :GameObject(parent, "TestModel"), hModel_(-1)
{
    
}

//デストラクタ
TestModel::~TestModel()
{
}

//初期化
void TestModel::Initialize()
{
    transform_.position_.x = (float)(rand() % 30 - 15) / 10;
    transform_.position_.z = +15;
    transform_.scale_.x = 0.5;
    transform_.scale_.y = 0.5;
    transform_.scale_.z = 0.5;
    //モデルデータのロード
    /*hModel_ = Model::Load("Assets\\oden.fbx");
    assert(hModel_ >= 0);*/
    
    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 0.8f);
    AddCollider(collision);
    
}

//更新
void TestModel::Update()
{
    
    //敵の動き
    transform_.position_.z -= 0.1;
    if (transform_.position_.z <= -15)
    {
        KillMe();
    }


}

//描画
void TestModel::Draw()
{

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void TestModel::Release()
{

}

void TestModel::OnCollision(GameObject* pTarget)
{
    //弾に当たったとき
    if (pTarget->GetObjectName() == "Ballet")
    {
        KillMe();
        pTarget->KillMe();
    }
}
