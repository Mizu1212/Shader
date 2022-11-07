#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

//-----------------------------------------------------------
//�J����
//-----------------------------------------------------------
namespace Camera
{
	//�������i�v���W�F�N�V�����s��쐬�j
	//���� �������E�B���h�E�̕��A�������E�B���h�E�̏c
	void Initialize(int WINDOW_WIDTH, int WINDOW_HEIGHT);

	//�X�V�i�r���[�s��쐬�j
	void Update();

	//���_�i�J�����̈ʒu�j��ݒ�
	//�����@�������J�����̈ʒu
	void SetPosition(XMVECTOR position);

	//�œ_�i����ʒu�j��ݒ�
	//�����@�������œ_�̈ʒu
	void SetTarget(XMVECTOR target);

	//�r���[�s����擾
	XMMATRIX GetViewMatrix();

	//�v���W�F�N�V�����s����擾
	XMMATRIX GetProjectionMatrix();

	//���_�i�J�����̈ʒu�j���擾
	XMVECTOR GetPosition();

	//�œ_�i����ʒu�j���擾
	XMVECTOR GetTarget();
};