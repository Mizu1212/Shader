#include "Camera.h"

namespace Camera
{
	//�ϐ�
	XMVECTOR position_;	//�J�����̈ʒu�i���_�j
	XMVECTOR target_;	//����ʒu�i�œ_�j
	XMMATRIX viewMatrix_;	//�r���[�s��
	XMMATRIX projMatrix_;	//�v���W�F�N�V�����s��
};

//������
void Camera::Initialize(int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
	position_ = XMVectorSet(0, 0, -5, 0);	//�J�����̈ʒu
	target_ = XMVectorSet(0, -2, 0, 0);	//�J�����̏œ_

	//�v���W�F�N�V�����s��
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f);//1��p�Q�c���̔䗦�R��O�̕\���͈�4�ǂ��܂ŕ`�悷�邩
}

//�X�V
void Camera::Update()
{

	//�r���[�s��̍쐬
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));
}

//�ʒu��ݒ�
void Camera::SetPosition(XMVECTOR position)
{
	 position_  = position;
}

//�œ_��ݒ�
void Camera::SetTarget(XMVECTOR target)
{
	target_ = target;
}

//�r���[�s����擾
XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

//�v���W�F�N�V�����s����擾
XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}

//�ʒu���擾
XMVECTOR Camera::GetPosition()
{
	return position_;
}

//�œ_���擾
XMVECTOR Camera::GetTarget()
{
	return target_;
}