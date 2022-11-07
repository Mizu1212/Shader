#pragma once
#include<vector>
#include<string>
#include"Spirete.h"
#include"Transform.h"

namespace Image
{

	struct ImageData
	{
		Spirete* pSpirete;      //Spirete�^�̃|�C���^

		std::string filename_; //�t�@�C���̖��O

		Transform transform_;  //transfrom

		//�R���X�g���N�^
		ImageData() : pSpirete(nullptr)
		{
		};

	};

	//������
	void Initialize();

	//�X�V
	int Load(std::string fileName);

	//�`��
	void Draw(int handle);

	//�J��
	void Release(int handle);//������

	//transform�̐ݒ�
	void SetTransform(int handle, Transform& transform_);
	//���[���h�s��̎擾
	XMMATRIX GetMatrix(int handle);
	//���ׂď���
	void AllRelease();
}

