#include "Image.h"
#include"Direct3D.h"
namespace Image
{
	//���[�h�ς݂̃��f���f�[�^�ꗗ
	std::vector<ImageData*>vi_Data;

	//������
	void Initialize()
	{
		AllRelease(); //���ׂĊJ��
	}

	//���f�������[�h
	int Load(std::string fileName)
	{
		ImageData* pIData = new ImageData;//�\���̂̃C���X�^���X���쐬

		//�J�����t�@�C���ꗗ���瓯���t�@�C�����̂��̂��������T��
		bool isExist = false;//�����t�@�C�������邩�ǂ������� Exist=����
		for (int i = 0; i < vi_Data.size(); i++)
		{
			//���łɊJ���Ă���ꍇ
			if (vi_Data[i] != nullptr && vi_Data[i]->filename_ == fileName)
			{
				pIData->pSpirete = vi_Data[i]->pSpirete;
				isExist = true;
				break;
			}
		}
		
		//�V���Ƀt�@�C�����J��
		if (isExist == false)
		{
			pIData->pSpirete = new Spirete;

			wchar_t wtext[FILENAME_MAX];
			size_t ret;
			mbstowcs_s(&ret, wtext, fileName.c_str(), strlen(fileName.c_str()));//�}���`�o�C�g���烏�C�h�ύX
			if (FAILED(pIData->pSpirete->Load(wtext)))
			{
				//�J���Ȃ�����
				SAFE_DELETE(pIData->pSpirete);
				SAFE_DELETE(pIData);
				return -1;
			}
			//�����J����
			pIData->filename_ = fileName;
		}

		//�V���ɒǉ�
		vi_Data.push_back(pIData); //�\���̂̒��g�����܂����̂œ��I�z��ɓ˂�����
		return (int)vi_Data.size() - 1;//�ԍ��i�z��̗v�f��-1�j��Ԃ�
	}

	void Draw(int handle)
	{
		//���f���̊m�F
		if (handle < 0 || handle >= vi_Data.size() || vi_Data[handle] == nullptr)
		{
			return;
		}

		if (vi_Data[handle]->pSpirete)
		{
			vi_Data[handle]->pSpirete->Draw(vi_Data[handle]->transform_,800,600);
		}
	}

	void Release(int handle)
	{
		//���f���̊m�F
		if (handle < 0 || handle >= vi_Data.size() || vi_Data[handle] == nullptr)
		{
			return;
		}
		//�������f���𑼂ł��g���Ă��Ȃ���
		bool isExist = false;//�g���ĂȂ����̑��݊m�F
		for (int i = 0; i < vi_Data.size(); i++)
		{
			//���łɊJ���Ă���ꍇ
			if (vi_Data[i] != nullptr && i != handle && vi_Data[i]->pSpirete == vi_Data[handle]->pSpirete)
			{
				isExist = true;
				break;
			}
		}
		//�g���ĂȂ���΃��f�����
		if (isExist == false)
		{
			SAFE_DELETE(vi_Data[handle]->pSpirete);
		}


		SAFE_DELETE(vi_Data[handle]);
	}

	void SetTransform(int handle, Transform& transform_)
	{
		if (handle < 0 || handle >= vi_Data.size())
		{
			return;
		}

		vi_Data[handle]->transform_ = transform_;
	}

	XMMATRIX GetMatrix(int handle)
	{
		return vi_Data[handle]->transform_.GetWorldMatrix();
	}

	void AllRelease()
	{
		for (int i = 0; i < vi_Data.size(); i++)
		{
			if (vi_Data[i] != nullptr)
			{
				Release(i);
			}
		}
		vi_Data.clear();
	}
}













