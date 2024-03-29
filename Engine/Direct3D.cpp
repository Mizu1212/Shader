#include "Direct3D.h"
#include <d3dcompiler.h>

#pragma region //変数
namespace Direct3D
{
	ID3D11Device* pDevice;		//デバイス
	ID3D11DeviceContext* pContext;		//デバイスコンテキスト
	IDXGISwapChain* pSwapChain;		//スワップチェイン
	ID3D11RenderTargetView* pRenderTargetView;	//レンダーターゲットビュー
	//Zバッファ
	ID3D11Texture2D* pDepthStencil;			//深度ステンシル
	ID3D11DepthStencilView* pDepthStencilView;		//深度ステンシルビュー

	ID3D11BlendState* pBlendState;
	Texture* pToonTexture;
	struct SHADER_BUNDLE
	{
		ID3D11VertexShader* pVertexShader = nullptr;	//頂点シェーダー
		ID3D11PixelShader* pPixelShader = nullptr;		//ピクセルシェーダー
		ID3D11InputLayout* pVertexLayout = nullptr;	//頂点インプットレイアウト
		ID3D11RasterizerState* pRasterizerState = nullptr;	//ラスタライザー
	};
	SHADER_BUNDLE shaderbundle[SHADER_MAX];
}
#pragma endregion

#pragma region //初期化
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{
	
	///////////////////////////いろいろ準備するための設定///////////////////////////////
	//いろいろな設定項目をまとめた構造体
	DXGI_SWAP_CHAIN_DESC scDesc;//スワップチェイン

	//とりあえず全部0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//描画先のフォーマット
	scDesc.BufferDesc.Width = winW;		//画面幅
	scDesc.BufferDesc.Height = winH;	//画面高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 何色使えるか

	//FPS（1/60秒に1回）
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//その他
	scDesc.Windowed = TRUE;			//ウィンドウモードかフルスクリーンか　TRUEがウィンドウ
	scDesc.OutputWindow = hWnd;		//ウィンドウハンドル
	scDesc.BufferCount = 1;			//バックバッファの枚数
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バックバッファの使い道＝画面に描画するために
	scDesc.SampleDesc.Count = 1;		//MSAA（アンチエイリアス）の設定
	scDesc.SampleDesc.Quality = 0;		//　〃

		////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成////////////////////////
	D3D_FEATURE_LEVEL level;
	D3D11CreateDeviceAndSwapChain(
		nullptr,				      // どのビデオアダプタを使用するか？既定ならばnullptrで
		D3D_DRIVER_TYPE_HARDWARE,	  // ドライバのタイプを渡す。ふつうはHARDWARE
		nullptr,				      // 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
		0,		                      // 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
		nullptr,				      // デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
		0,					          // 上の引数でレベルを何個指定したか
		D3D11_SDK_VERSION,			  // SDKのバージョン。必ずこの値
		&scDesc,				      // 上でいろいろ設定した構造体
		&pSwapChain,				  // 無事完成したSwapChainのアドレスが返ってくる
		&pDevice,				      // 無事完成したDeviceアドレスが返ってくる
		&level,					      // 無事完成したDevice、Contextのレベルが返ってくる
		&pContext);				      // 無事完成したContextのアドレスが返ってくる

	///////////////////////////レンダーターゲットビュー作成///////////////////////////////
	//スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）
	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	HRESULT hr;
	//レンダーターゲットビューを作成
	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "レンダーターゲットビューの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pBackBuffer);
		return hr;
	}

	//一時的にバックバッファを取得しただけなので解放
	SAFE_RELEASE(pBackBuffer);

	///////////////////////////ビューポート（描画範囲）設定///////////////////////////////
	//レンダリング結果を表示する範囲
	D3D11_VIEWPORT vp;
	vp.Width = (float)winW;	 //幅
	vp.Height = (float)winH;//高さ
	vp.MinDepth = 0.0f;	             //手前
	vp.MaxDepth = 1.0f;	             //奥
	vp.TopLeftX = 0;	             //左
	vp.TopLeftY = 0;	             //上

	//深度ステンシルビューの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = winW;
	descDepth.Height = winH;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	pDevice->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);


	//ブレンドステート
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;


	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;  //今から表示するもの
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//もう表示されているもの
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pDevice->CreateBlendState(&BlendDesc, &pBlendState);

	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	pContext->OMSetBlendState(pBlendState, blendFactor, 0xffffffff);







	//データを画面に描画するための一通りの設定（パイプライン）
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // データの入力種類を指定
	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);            // 描画先を設定
	pContext->RSSetViewports(1, &vp);

	/*pToonTexture = new Texture;
	pToonTexture->Load(L"Assets\\Toon.png");*/
	
	//シェーダー準備
	hr = InitShader();
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;

}
#pragma endregion

//シェーダー準備
HRESULT Direct3D::InitShader()
{
	HRESULT hr;
	
	//3D
	hr = InitShader3D();
	if (FAILED(hr))
	{
		return hr;
	}
	//2D
	hr = InitShader2D();
	if (FAILED(hr))
	{
		return hr;
	}
	hr = InitShaderWATER();
	if (FAILED(hr))
	{
		return hr;
	}
	hr = InitShaderTEST();
	if (FAILED(hr))
	{
		return hr;
	}
	hr = InitShaderTOON();
	if (FAILED(hr))
	{
		return hr;
	}
	hr = InitShaderOUTLINE();
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT Direct3D::InitShader3D()
{
	HRESULT hr;


	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;//コンパイルした結果が入る
	D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);

	 hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderbundle[SHADER_3D].pVertexShader);
	 if (FAILED(hr))
	 {
		 MessageBox(NULL, "頂点シェーダーの作成に失敗しました", "エラー", MB_OK);
		 SAFE_RELEASE(pCompileVS);
		 return hr;
	 }

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR)*0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(XMVECTOR)*1,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//UV
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR)*2,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
		//{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR)*3,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
	};
	hr = pDevice->CreateInputLayout(layout, sizeof(layout)/sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderbundle[SHADER_3D].pVertexLayout);//第二引数には上で作った個数が入る
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点インプットレイアウトの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompileVS);
		return hr;
	}

	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderbundle[SHADER_3D].pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダーの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompilePS);
		return hr;
	}

	SAFE_RELEASE(pCompilePS);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderbundle[SHADER_3D].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ラスタライザの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;
}


HRESULT Direct3D::InitShader2D()
{
	HRESULT hr;
	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;//コンパイルした結果が入る
	D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);

	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderbundle[SHADER_2D].pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点シェーダーの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompileVS);
		return hr;
	}

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV座標
	};
	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderbundle[SHADER_2D].pVertexLayout);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点インプットレイアウトの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompileVS);
		return hr;
	}

	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderbundle[SHADER_2D].pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダーの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompilePS);
		return hr;
	}

	SAFE_RELEASE(pCompilePS);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderbundle[SHADER_2D].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ラスタライザの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;


	
}

HRESULT Direct3D::InitShaderWATER()
{
	HRESULT hr;


	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;//コンパイルした結果が入る
	D3DCompileFromFile(L"WaterShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);

	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderbundle[SHADER_WATER].pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点シェーダーの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompileVS);
		return hr;
	}

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(XMVECTOR) * 1,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//UV
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 3,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
	};
	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderbundle[SHADER_WATER].pVertexLayout);//第二引数には上で作った個数が入る
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点インプットレイアウトの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompileVS);
		return hr;
	}

	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"WaterShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderbundle[SHADER_WATER].pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダーの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompilePS);
		return hr;
	}

	SAFE_RELEASE(pCompilePS);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderbundle[SHADER_WATER].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ラスタライザの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Direct3D::InitShaderTEST()
{
	HRESULT hr;


	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;//コンパイルした結果が入る
	D3DCompileFromFile(L"NormalMap.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);

	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderbundle[SHADER_TEST].pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点シェーダーの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompileVS);
		return hr;
	}

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(XMVECTOR) * 1,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//UV
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 3,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
	};
	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderbundle[SHADER_TEST].pVertexLayout);//第二引数には上で作った個数が入る
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点インプットレイアウトの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompileVS);
		return hr;
	}

	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"NormalMap.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderbundle[SHADER_TEST].pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダーの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompilePS);
		return hr;
	}

	SAFE_RELEASE(pCompilePS);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderbundle[SHADER_TEST].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ラスタライザの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Direct3D::InitShaderTOON()
{
	HRESULT hr;


	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;//コンパイルした結果が入る
	D3DCompileFromFile(L"Toon.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);

	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderbundle[SHADER_TOON].pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点シェーダーの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompileVS);
		return hr;
	}

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(XMVECTOR) * 1,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//UV
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
		//{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 3,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
	};
	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderbundle[SHADER_TOON].pVertexLayout);//第二引数には上で作った個数が入る
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点インプットレイアウトの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompileVS);
		return hr;
	}

	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Toon.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderbundle[SHADER_TOON].pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダーの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompilePS);
		return hr;
	}

	SAFE_RELEASE(pCompilePS);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderbundle[SHADER_TOON].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ラスタライザの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Direct3D::InitShaderOUTLINE()
{
	HRESULT hr;


	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;//コンパイルした結果が入る
	D3DCompileFromFile(L"Toon.hlsl", nullptr, nullptr, "VS_Outline", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);

	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderbundle[SHADER_OUTLINE].pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点シェーダーの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompileVS);
		return hr;
	}

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(XMVECTOR) * 1,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//UV
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
		//{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 3,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
	};
	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderbundle[SHADER_OUTLINE].pVertexLayout);//第二引数には上で作った個数が入る
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点インプットレイアウトの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompileVS);
		return hr;
	}

	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Toon.hlsl", nullptr, nullptr, "PS_Outline", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderbundle[SHADER_OUTLINE].pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ピクセルシェーダーの作成に失敗しました", "エラー", MB_OK);
		SAFE_RELEASE(pCompilePS);
		return hr;
	}

	SAFE_RELEASE(pCompilePS);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderbundle[SHADER_OUTLINE].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "ラスタライザの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

//シェーダーを設定
void Direct3D::SetShader(SHADER_TYPE type)
{
	//それぞれをデバイスコンテキストにセット
	pContext->VSSetShader(shaderbundle[type].pVertexShader, NULL, 0);	//頂点シェーダー
	pContext->PSSetShader(shaderbundle[type].pPixelShader, NULL, 0);	//ピクセルシェーダー
	pContext->IASetInputLayout(shaderbundle[type].pVertexLayout);	//頂点インプットレイアウト
	pContext->RSSetState(shaderbundle[type].pRasterizerState);		//ラスタライザー
}


#pragma region //描画開始
void Direct3D::BeginDraw()
{
	//背景の色
	float clearColor[4] = { 1.0f, 0.5f, 0.5f, 1.0f };//R,G,B,A

	//画面をクリア
	pContext->ClearRenderTargetView(pRenderTargetView, clearColor);

	//深度バッファクリア
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
#pragma endregion

#pragma region //描画終了
void Direct3D::EndDraw()
{
	//スワップ（バックバッファを表に表示する）
	pSwapChain->Present(0, 0);
	
}
#pragma endregion

#pragma region //解放処理
void Direct3D::Release()
{
	for (int i = 0; i < SHADER_MAX; i++)
	{
		SAFE_RELEASE(shaderbundle[i].pRasterizerState);
		SAFE_RELEASE(shaderbundle[i].pVertexLayout);
		SAFE_RELEASE(shaderbundle[i].pPixelShader);
		SAFE_RELEASE(shaderbundle[i].pVertexShader);
	}
	//解放処理
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pSwapChain);
	SAFE_RELEASE(pContext);
	SAFE_RELEASE(pDevice);
}
#pragma endregion


