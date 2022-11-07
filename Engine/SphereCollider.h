#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include"Transform.h"
using namespace DirectX;

//-----------------------------------------------------------
//���̂̓����蔻��
//-----------------------------------------------------------



class SphereCollider
{
protected:
	//GameObject* pGameObject_;	//���̔���������Q�[���I�u�W�F�N�g
	XMFLOAT3		center_;		//���S�ʒu�i�Q�[���I�u�W�F�N�g�̌��_���猩���ʒu�j
	float radius;

	//int				hDebugModel_;	//�f�o�b�O�\���p�̃��f����ID
public:
	//�R���X�g���N�^�i�����蔻��̍쐬�j
	//�����Fcenter	�����蔻��̒��S�ʒu�i�Q�[���I�u�W�F�N�g�̌��_���猩���ʒu�j
	//�����Fradius	�����蔻��̃T�C�Y�i���a�j
	SphereCollider(XMFLOAT3 center, float radius_);

	
	//�R���C�_�[���擾
	float GetCollider();
};

