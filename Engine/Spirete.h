#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include"Transform.h"

#define SAFE_DELETE_ARRAY(p) if(p != nullptr){ delete[] p; p = nullptr;}
//2D�摜��`�悷��N���X
class Spirete
{
protected:
	//�R���X�^���g�o�b�t�@�[
	struct CONSTANT_BUFFER
	{
		//���[���h�s��
		XMMATRIX	matW;

	};

	//���_���
	struct VERTEX
	{
		XMVECTOR position;	//�ʒu
		XMVECTOR uv;		//UV

	};
	DWORD	vertexNum_;		//���_��
	VERTEX* vertices_;		//���_���
	ID3D11Buffer* pVertexBuffer_;		//���_�o�b�t�@
	DWORD indexNum;			//�C���f�b�N�X��
	int* index_;			//�C���f�b�N�X���
	ID3D11Buffer* pIndexBuffer_;		//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_;		//�e�N�X�`��
	UINT WIDTH;             //�E�B���h�E�̃T�C�Y
	UINT HEIGHT;            //�E�B���h�E�̃T�C�Y

public:
	Spirete();
	~Spirete();
	
	//�������i�|���S����\�����邽�߂̊e����������j
	//�ߒl�F�����^���s
	HRESULT Load(LPCWSTR fileName);
	//�`��
	//�����FworldMatrix	���[���h�s��
	void Draw(Transform transform, int WINDOW_WIDTH, int WINDOW_HEIGHT);
	//���
	void Release();
private:
	//---------Initialize����Ă΂��֐�---------
	virtual void InitVertexData();		//���_���̏���
	HRESULT CreateVertexBuffer();		//���_�o�b�t�@���쐬
	virtual void InitIndexData();		//�C���f�b�N�X��������
	HRESULT CreateIndexBuffer();		//�C���f�b�N�X�o�b�t�@���쐬
	HRESULT CreateConstantBuffer();		//�R���X�^���g�o�b�t�@�쐬

	HRESULT LoadTexture(LPCWSTR fileName);				//�e�N�X�`�������[�h
	//---------Draw�֐�����Ă΂��֐�---------
	void PassDataToCB(DirectX::XMMATRIX worldMatrix,int WINDOW_WIDTH,int WINDOW_HEIGHT);	//�R���X�^���g�o�b�t�@�Ɋe�����n��
	void SetBufferToPipeline();							//�e�o�b�t�@���p�C�v���C���ɃZ�b�g
};