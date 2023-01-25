#pragma once
#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include"Macro.h"
#include"Direct3D.h"
//リンカ
#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")

class Texture;//ポインタだけであればこれだけでおｋ 前方宣言

class Fbx
{
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;   //Texture型のポインタ
		Texture* pTextureNormal;   //Texture型のポインタ
		XMFLOAT4	diffuse; //マテリアル　元のカラー
		XMFLOAT4    ambient;
		XMFLOAT4    specular;
		float   shiness;
		
	};

	//コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		XMMATRIX matWVP;
		XMMATRIX matNormal;
		XMMATRIX matW;
		XMFLOAT4 diffusecolor;
		XMFLOAT4 ambientcolor;
		XMFLOAT4 specularcolor;
		XMFLOAT4 camPos;
		float shiness;
		float scroll;
		float       movepos;
		int      isTexture;
		
		
		//XMMATRIX	matWVP;       // ワールド・ビュー・プロジェクションの合成行列
		//XMMATRIX	matNormal;    //法線
		//XMFLOAT4    diffuseColor; //マテリアルの色
		//int		isTexture;		  // テクスチャ貼ってあるかどうか
	};
	
	//頂点
	struct VERTEX
	{
		XMVECTOR position; //ポジション
		XMVECTOR uv;       //テクスチャの座標
		XMVECTOR normal;   //法線
		XMVECTOR tangent;
	};

	Texture* pTexToon_;

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	ID3D11Buffer* pConstantBuffer_2;
	MATERIAL* pMaterialList_; //MATERIAL型のポインタ
	SHADER_TYPE Type_;
public:
	//コンストラクタ
	Fbx();                
	//ロード
	//引数　第一引数ファイルの名前
	HRESULT Load(std::string fileName, SHADER_TYPE Type_);
	//描画
	//引数　第一引数Transform型のポインタ
	void    Draw(Transform& transform);
	//解放
	void    Release();                 
private:
	//頂点バッファ準備
	//引数　第一引数FbxMesh型のポインタ
	HRESULT InitVertex(fbxsdk::FbxMesh* pMesh);
	//インデックスバッファ準備
	//引数　第一引数FbxMesh型のポインタ
	HRESULT InitIndex(fbxsdk::FbxMesh* pMesh);
	//コンスタントバッファ準備
	HRESULT IntConstantBuffer();
	//マテリアル準備
	//引数　第一引数FbxMesh型のポインタ
	HRESULT InitMaterial(fbxsdk::FbxNode* pNode);
	
	
};

