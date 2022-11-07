#pragma once
#include <d3d11.h>
#include "string"

class Texture
{
	ID3D11SamplerState* pSampler_;  //�e�N�X�`���̓\��������߂�
	ID3D11ShaderResourceView* pSRV_;//�e�N�X�`�����V�F�[�_�[�ɓn�����߂̃r���[���쐬����
	UINT imgWidth;
	UINT imgHeight;
public:
	Texture();
	~Texture();
	HRESULT Load(LPCWSTR fileName);
	void Release();
	UINT GetWidth(){return imgWidth; }//�����擾
	UINT GetHeight() { return imgHeight; }//���擾
	ID3D11SamplerState* GetSampler() { return pSampler_; }//�T���v���[�擾
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }//�r���[�擾
};