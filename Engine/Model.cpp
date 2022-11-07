#include "Model.h"

namespace Model
{
	//���[�h�ς݂̃��f���f�[�^�ꗗ
	std::vector<ModelData*> vm_Data;

	//������
	void Initialize()
	{
		AllRelease();
	}

	//���f�������[�h
	int Load(std::string fileName)
	{
		ModelData* pMData = new ModelData;//�\���̂̃C���X�^���X���쐬

		//�J�����t�@�C���ꗗ���瓯���t�@�C�����̂��̂��������T��
		bool isExist = false;//�����t�@�C�������邩�ǂ������� Exist=����
		for (int i = 0; i < vm_Data.size(); i++)
		{
			//���łɊJ���Ă���ꍇ
			if (vm_Data[i] != nullptr && vm_Data[i]->filename_ == fileName)//vm_Data�Ƀf�[�^�������Ă��邩�����Ă���t�@�C���̂Ȃ܂��������Ƃ�
			{
				pMData->pFbx = vm_Data[i]->pFbx;
				isExist = true;
				break;
			}
		}

		//�V���Ƀt�@�C�����J��
		if (isExist == false)
		{
			pMData->pFbx = new Fbx;//Fbx�I�u�W�F�N�g���쐬���A���[�h����
			if (FAILED(pMData->pFbx->Load(fileName)))
			{
				//�J���Ȃ�����
				SAFE_DELETE(pMData->pFbx);
				SAFE_DELETE(pMData);
				return -1;
			}
			//�����J����
			pMData->filename_ = fileName;
		}

		//�g���ĂȂ��ԍ����������T��
		for(int i = 0; i < vm_Data.size(); i++)
		{
			if (vm_Data[i] == nullptr)
			{
				vm_Data[i] = pMData;
				return i;
			}
		}
		//�V���ɒǉ�
		vm_Data.push_back(pMData); //�\���̂̒��g�����܂����̂œ��I�z��ɓ˂�����
		return (int)vm_Data.size() - 1;//�ԍ��i�z��̗v�f��-1�j��Ԃ�

	}
	void Draw(int handle)
	{
		//���f���̊m�F
		if (handle < 0 || handle >= vm_Data.size() || vm_Data[handle] == nullptr)
		{
			return;
		}

		if (vm_Data[handle]->pFbx)
		{
			vm_Data[handle]->pFbx->Draw(vm_Data[handle]->transform_);
		}

	}
	//�C�ӂ̃��f�����J��
	void Release(int handle)
	{
		//���f���̊m�F
		if (handle < 0 || handle >= vm_Data.size() || vm_Data[handle] == nullptr)
		{
			return;
		}
		//�������f���𑼂ł��g���Ă��Ȃ���
		bool isExist = false;//�g���ĂȂ����̑��݊m�F
		for (int i = 0; i < vm_Data.size(); i++)
		{
			//���łɊJ���Ă���ꍇ
			if (vm_Data[i] != nullptr && i != handle && vm_Data[i]->pFbx == vm_Data[handle]->pFbx)
			{
				isExist = true;
				break;
			}
		}
		//�g���ĂȂ���΃��f�����
		if (isExist == false)
		{
			SAFE_DELETE(vm_Data[handle]->pFbx);
		}


		SAFE_DELETE(vm_Data[handle]);
	}
	//���[���h�s���ݒ�
	void SetTransform(int handle, Transform& transform_)
	{
		if (handle < 0 || handle >= vm_Data.size())
		{
			return;
		}

		vm_Data[handle]->transform_ = transform_;
	}
	//���[���h�s��̎擾
	XMMATRIX GetMatrix(int handle)
	{
		return vm_Data[handle]->transform_.GetWorldMatrix();
	}
	//�S�Ẵ��f�������
	void AllRelease()
	{
		for (int i = 0; i < vm_Data.size(); i++)
		{
			if (vm_Data[i] != nullptr)
			{
				Release(i);
			}
		}
		vm_Data.clear();
	}
}