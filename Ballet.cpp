#include "Ballet.h"
#include"Engine/Input.h"
#include "Engine/Model.h"
//コンストラクタ
Ballet::Ballet(GameObject* parent)
    :GameObject(parent, "Ballet"), hModel_(-1)
{
}

//デストラクタ
Ballet::~Ballet()
{
}

//初期化
void Ballet::Initialize()
{
    transform_.scale_.x = 0.3;
    transform_.scale_.y = 0.3;
    transform_.scale_.z = 0.3;
    
    ////モデルデータのロード
    //hModel_ = Model::Load("Assets\\oden.fbx");
    //assert(hModel_ >= 0);

    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
    AddCollider(collision);
}

//更新
void Ballet::Update()
{
    //弾の動き
    transform_.position_.z += 0.2;
    if ( transform_.position_.z >= 20 )
    {
        KillMe();
    }
}

//描画
void Ballet::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}


void Ballet::Release()
{
}