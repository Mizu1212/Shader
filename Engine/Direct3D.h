#pragma once
#include <d3d11.h>
#include <assert.h>
#include <DirectXMath.h>
#include"Macro.h"
using namespace DirectX;

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//シェードのタイプ
enum SHADER_TYPE 
{
	SHADER_2D,
	SHADER_3D,
	SHADER_WATER,
	SHADER_TEST,
	SHADER_TOON,
	SHADER_OUTLINE,
	SHADER_MAX
};



namespace Direct3D
{

	extern ID3D11Device* pDevice;		//デバイス
	extern ID3D11DeviceContext* pContext;	//デバイスコンテキスト

	//初期化
	//引数　第一引数ウィンドウの幅、第二引数ウィンドウの縦、第三引数ハンドルウィンドウ
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
	HRESULT InitShader();
	HRESULT InitShader3D();
	HRESULT InitShader2D();
	HRESULT InitShaderWATER();
	HRESULT InitShaderTEST();
	

	//シェーダーを設定
	//引数　第一引数シェードのタイプ
	void SetShader(SHADER_TYPE type);

	//描画開始
	void BeginDraw();

	//描画終了
	void EndDraw();

	//解放
	void Release();

};