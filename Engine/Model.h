#pragma once
#include<vector>
#include<string>
#include"Fbx.h"
#include"Transform.h"
#include "Direct3D.h"
namespace Model
{
	struct ModelData
	{
		Fbx* pFbx;//Fbx型のポインタ

		Transform transform_;//Transform型の変数

		std::string filename_;//ファイル名を入れる変数

        ModelData() : pFbx(nullptr)
        {
        };
	};

   

    //初期化
    void Initialize();

    //更新
    int Load(std::string fileName, SHADER_TYPE Type_);

    //描画
    void Draw(int handle);

    //開放
    void Release(int handle);

    //transformの設定
    void SetTransform(int handle, Transform& transform_);
    //ワールド行列の取得
    XMMATRIX GetMatrix(int handle);
    //すべて消す
    void AllRelease();
}

