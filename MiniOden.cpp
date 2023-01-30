#include "MiniOden.h"
#include "Engine/Model.h"

//コンストラクタ
MiniOden::MiniOden(GameObject* parent)
    :GameObject(parent,"MiniOden"), hModel_(-1)
{
}

//デストラクタ
MiniOden::~MiniOden()
{
}

//初期化
void MiniOden::Initialize()
{
    transform_.position_.x = +1;
    transform_.position_.z = -1;
    transform_.scale_.x = 0.3;
    transform_.scale_.y = 0.3;
    transform_.scale_.z = 0.3;
    //モデルデータのロード
   /* hModel_ = Model::Load("Assets\\oden.fbx");
    assert(hModel_ >= 0);*/
}

//更新
void MiniOden::Update()
{
}

//描画
void MiniOden::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void MiniOden::Release()
{
}