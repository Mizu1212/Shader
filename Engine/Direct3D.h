#pragma once
#include <d3d11.h>
#include <assert.h>
#include <DirectXMath.h>
#include"Macro.h"
#include"Texture.h"
using namespace DirectX;

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//�V�F�[�h�̃^�C�v
enum SHADER_TYPE 
{
	SHADER_3D,
	SHADER_2D,
	SHADER_TEST,
	SHADER_WATER,
	SHADER_TOON,
	SHADER_OUTLINE,
	SHADER_MAX
};



namespace Direct3D
{

	extern ID3D11Device* pDevice;		//�f�o�C�X
	extern ID3D11DeviceContext* pContext;	//�f�o�C�X�R���e�L�X�g
	extern Texture* pToonTexture;
	//������
	//�����@�������E�B���h�E�̕��A�������E�B���h�E�̏c�A��O�����n���h���E�B���h�E
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//�V�F�[�_�[����
	HRESULT InitShader();
	HRESULT InitShader3D();
	HRESULT InitShader2D();
	HRESULT InitShaderWATER();
	HRESULT InitShaderTEST();
	HRESULT InitShaderTOON();
	HRESULT InitShaderOUTLINE();

	//�V�F�[�_�[��ݒ�
	//�����@�������V�F�[�h�̃^�C�v
	void SetShader(SHADER_TYPE type);

	//�`��J�n
	void BeginDraw();

	//�`��I��
	void EndDraw();

	//���
	void Release();

};