#include "SphereCollider.h"

//�R���X�g���N�^�i�����蔻��̍쐬�j
//�����Fcenter	�����蔻��̒��S�ʒu�i�Q�[���I�u�W�F�N�g�̌��_���猩���ʒu�j
	//�����Fradius	�����蔻��̃T�C�Y�i���a�j
SphereCollider::SphereCollider(XMFLOAT3 center, float radius_)
{
	center_ = center;
	radius = radius_;
}

float SphereCollider::GetCollider()
{
	return radius;
}


