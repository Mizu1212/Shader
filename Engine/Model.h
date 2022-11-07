#pragma once
#include<vector>
#include<string>
#include"Fbx.h"
#include"Transform.h"
namespace Model
{
	struct ModelData
	{
		Fbx* pFbx;//Fbx�^�̃|�C���^

		Transform transform_;//Transform�^�̕ϐ�

		std::string filename_;//�t�@�C����������ϐ�

        ModelData() : pFbx(nullptr)
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
    void Release(int handle);

    //transform�̐ݒ�
    void SetTransform(int handle, Transform& transform_);
    //���[���h�s��̎擾
    XMMATRIX GetMatrix(int handle);
    //���ׂď���
    void AllRelease();
}

