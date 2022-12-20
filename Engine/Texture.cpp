#include "Texture.h"
#include <wincodec.h>
#include"Direct3D.h"

#pragma comment( lib, "WindowsCodecs.lib" )

Texture::Texture()
{
	pSampler_ = nullptr;
	pSRV_ = nullptr;
	imgHeight = NULL;
	imgWidth = NULL;

}
Texture::~Texture()
{
}

HRESULT Texture::Load(LPCWSTR fileName)
{
	HRESULT hr;
	//画像ファイルロード
	IWICImagingFactory* pFactory = nullptr;
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	IWICFormatConverter* pFormatConverter = nullptr;
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&pFactory));
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	hr = pFactory->CreateDecoderFromFilename(fileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	hr = pDecoder->GetFrame(0, &pFrame);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	hr = pFactory->CreateFormatConverter(&pFormatConverter);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	hr = pFormatConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 1.0f, WICBitmapPaletteTypeMedianCut);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	//画像のサイズを調べる
	
	hr = pFormatConverter->GetSize(&imgWidth, &imgHeight);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "画像のサイズ取得に失敗しました", "エラー", MB_OK);
		return E_FAIL;
	}

	//テクスチャの作成
	ID3D11Texture2D* pTexture;//テクスチャを扱う型
	D3D11_TEXTURE2D_DESC texdec;
	texdec.Width = imgWidth;
	texdec.Height = imgHeight;
	texdec.MipLevels = 1;
	texdec.ArraySize = 1;
	texdec.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texdec.SampleDesc.Count = 1;
	texdec.SampleDesc.Quality = 0;
	texdec.Usage = D3D11_USAGE_DYNAMIC;
	texdec.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texdec.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texdec.MiscFlags = 0;
	hr = Direct3D::pDevice->CreateTexture2D(&texdec, nullptr, &pTexture);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "テクスチャの作成に失敗しました", "エラー", MB_OK);
		return E_FAIL;
	}

	//テクスチャをコンテキストに渡す
	D3D11_MAPPED_SUBRESOURCE hMappedres;
	//停止
	hr = Direct3D::pContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedres);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	//コピー
	hr = pFormatConverter->CopyPixels(nullptr, imgWidth * 4, imgWidth * imgHeight * 4, (BYTE*)hMappedres.pData);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "テクスチャのコピーに失敗しました", "エラー", MB_OK);
		return E_FAIL;
	}
	//再開
	Direct3D::pContext->Unmap(pTexture, 0);

	//サンプラー作成
	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	                 //D3D11_FILTER_MIN_MAG_MIP_POINT;

	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = Direct3D::pDevice->CreateSamplerState(&SamDesc, &pSampler_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "サンプラーの作成に失敗しました", "エラー", MB_OK);
		return E_FAIL;
	}

	//シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	hr = Direct3D::pDevice->CreateShaderResourceView(pTexture, &srv, &pSRV_);
	if (FAILED(hr))
	{
		MessageBox(nullptr,"シェーダーリソースの作成に失敗しました", "エラー", MB_OK);
		return E_FAIL;
	}
	pTexture->Release();

	return S_OK;
}

void Texture::Release()
{
	SAFE_RELEASE(pSRV_);
	SAFE_RELEASE(pSampler_);
}