#pragma once
#include <string>
#include <list>
#include "Transform.h"
#include"SphereCollider.h"



class GameObject
{
private:
	bool deadf = false; //����ł��邩�̃t���O
protected:
	std::list<GameObject*> childList_;//�Q�[���I�u�W�F�N�g�̃��X�g�\��
	Transform	transform_;//transform�N���X
	GameObject* pParent_;//�Q�[���I�u�W�F�N�g�Ȃ�N�ł��i�|�C���^�j
	std::string	objectName_;//������
	SphereCollider* pSphereCollider_;//SphereCollider�^�̃|�C���^

	

public:
	//�R���X�g���N�^
	GameObject();
	//�����t���R���X�g���N�^
	//�����@������GameObject�^�̃|�C���^
	GameObject(GameObject* parent);
	//�����t���R���X�g���N�^
	//�����@������GameObject�^�̃|�C���^�@�������@string�^�̃|�C���^
	GameObject(GameObject* parent, const std::string& name);
	//�f�X�g���N�^
	virtual ~GameObject();


	virtual void Initialize() = 0;//�������z�֐�
	virtual void Update() = 0;    //�������z�֐�
	virtual void Draw() = 0;      //�������z�֐�
	virtual void Release() = 0;   //�������z�֐�
	
	//�e���v���[�g
	template <class T>
	void Instantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		p->Initialize();
		parent->childList_.push_back(p);
	}

	template <class T>
	GameObject* PInstantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		p->Initialize();
		parent->childList_.push_back(p);

		return p;
	}

	//�q�I�u�W�F�N�g���X�g���擾
	std::list<GameObject*>* GetChildList();

	//�|�W�V�����̃Z�b�g
	void SetPosition(XMFLOAT3 position);
	
	void DrawSub();
	void UpdateSub();
	void ReleaseSub();
	
	XMMATRIX GetWorldMatrix();

	//����ł��邩������
	bool IsDed();

	//�폜
	void KillMe();

	//�q�I�u�W�F�N�g��S�č폜
	void KillAllChildren();

	//���O�ŃI�u�W�F�N�g�������i�Ώۂ͎����̎q���ȉ��j
	//�����@������string�^�̃|�C���^
	GameObject* FindChildObject(const std::string& name);

	//���O�ŃI�u�W�F�N�g�������i�Ώۂ͑S�́j
	//�����@������string�^�̃|�C���^
	GameObject* FindObject(const std::string& name) { return GetRootJob()->FindChildObject(name); }

	//RootJob���擾
	GameObject* GetRootJob();

	//�e�I�u�W�F�N�g���擾
	GameObject* GetParent();

	//�I�u�W�F�N�g�̖��O���擾
	const std::string& GetObjectName(void) const;


	//XMFLOAT3 GetWorldPosition() { return Transform::Float3Add(GetParent()->transform_.position_, transform_.position_); }


	//�R���C�_�[�i�Փ˔���j��ǉ�����
	//�����@������SphereCollider�^�̃|�C���^
	void AddCollider(SphereCollider* collider) { pSphereCollider_ = collider; }

	//��������
	//�����@������GameObject�^�̃|�C���^
	void Oncollision(GameObject* pTarget);

	//�����ƏՓ˂����ꍇ�ɌĂ΂��i�I�[�o�[���C�h�p�j
	//�����FpTarget	�Փ˂�������
	virtual void OnCollision(GameObject* pTarget) {};

	//�Փ˔���
	//�����FpTarget	�Փ˂��Ă邩���ׂ鑊��
	void Collision(GameObject* pTarget);
private:
	//�I�u�W�F�N�g�폜�i�ċA�j
	void KillObjectSub(GameObject* obj);
};