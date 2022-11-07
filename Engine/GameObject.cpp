#include "GameObject.h"


GameObject::GameObject()
{
}

GameObject::GameObject(GameObject* parent)
{
	pParent_ = nullptr;
	objectName_ = "";
}

GameObject::GameObject(GameObject* parent, const std::string& name)
{
	pParent_  = parent;
	objectName_ = name;
	if(parent!=nullptr)
	transform_.pParent_ = &parent->transform_;
}

GameObject::~GameObject()
{
}

//�q�I�u�W�F�N�g���X�g���擾
std::list<GameObject*>* GameObject::GetChildList()
{
	return &childList_;
}

void GameObject::SetPosition(XMFLOAT3 position)
{
	transform_.position_ = position;
}

void GameObject::DrawSub()
{
	Draw();

	for (auto i = childList_.begin();i != childList_.end();i++)
	{
		(*i)->DrawSub();
	}
}

void GameObject::UpdateSub()
{
	Update();
	Oncollision(GetRootJob());

	for (auto i = childList_.begin(); i != childList_.end(); i++)
	{
		(*i)->UpdateSub();
		
	}
	for (auto i = childList_.begin(); i != childList_.end();)
	{
		if ((*i)->IsDed())
		{
			(*i)->ReleaseSub();
			delete(*i);
			i = childList_.erase(i);
		}
		else
		{
			i++;
		}

	}
}

void GameObject::ReleaseSub()
{
	Release();
    for (auto i = childList_.begin(); i != childList_.end(); i++)
	{
			(*i)->ReleaseSub();
	}
}

XMMATRIX GameObject::GetWorldMatrix(void)
{
	return transform_.GetWorldMatrix();
	
}

bool GameObject::IsDed()
{
	return deadf ;
}

void GameObject::KillMe()
{
	deadf = true;
}

//�q�I�u�W�F�N�g��S�č폜
void GameObject::KillAllChildren(void)
{
	//�q�������Ȃ��Ȃ�I���
	if (childList_.empty())
		return;

	//�C�e���[�^
	auto it = childList_.begin();	//�擪
	auto end = childList_.end();	//����

	//�q�I�u�W�F�N�g��1���폜
	while (it != end)
	{
		KillObjectSub(*it);
		delete* it;
		it = childList_.erase(it);
	}

	//���X�g���N���A
	childList_.clear();
}
//���O�ŃI�u�W�F�N�g�������i�Ώۂ͎����̎q���ȉ��j
GameObject* GameObject::FindChildObject(const std::string& name)
{
	//�q�������Ȃ��Ȃ�I���
	if (childList_.empty())
		return nullptr;

	//�C�e���[�^
	auto it = childList_.begin();	//�擪
	auto end = childList_.end();	//����

	//�q�I�u�W�F�N�g����T��
	while (it != end) {
		//�������O�̃I�u�W�F�N�g���������炻���Ԃ�
		if ((*it)->GetObjectName() == name)
			return *it;

		//���̎q���i���j�ȍ~�ɂ��Ȃ����T��
		GameObject* obj = (*it)->FindChildObject(name);
		if (obj != nullptr)
		{
			return obj;
		}

		//���̎q��
		it++;
	}

	//������Ȃ�����
	return nullptr;
}

GameObject* GameObject::GetRootJob()
{
	if (GetParent() == nullptr)
	{
		return this;
	}
	else return GetParent()->GetRootJob();
}

//�e�I�u�W�F�N�g���擾
GameObject* GameObject::GetParent(void)
{
	return pParent_;
}

//�I�u�W�F�N�g�̖��O���擾
const std::string& GameObject::GetObjectName(void) const
{
	return objectName_;
}

//��������
void GameObject::Oncollision(GameObject* pTarget)
{
	Collision(pTarget);
	for (auto i = pTarget->GetChildList()->begin(); i != pTarget->GetChildList()->end(); i++)
	{
		Oncollision(*i);
	}
}

//void GameObject::AddCollider(SphereCollider* collider)
//{
//	collider->SetGameObject(this);
//	scolliderList_.push_back(collider);
//}

//�Փ˔���
void GameObject::Collision(GameObject* pTarget)
{
	//�������m�̓����蔻��͂��Ȃ�
	if (pTarget == nullptr || this == pTarget)
	{
		return;
	}

	float x =0.0;
	float y = 0.0;
	float z = 0.0;
	float K = 0.0;
	float L = 0.0;
	x = this->transform_.position_.x - pTarget->transform_.position_.x;
	y = this->transform_.position_.y - pTarget->transform_.position_.y;
	z = this->transform_.position_.z - pTarget->transform_.position_.z;
	K = (x * x + y * y + z * z);
	if (this->pSphereCollider_ != nullptr&& pTarget->pSphereCollider_!=nullptr)
	{
		L = this->pSphereCollider_->GetCollider() + pTarget->pSphereCollider_->GetCollider();
	}
	else
	{
		return;
	}

	if (K<=(L*L))
	{
		OnCollision(pTarget);
	}
}

//�I�u�W�F�N�g�폜�i�ċA�j
void GameObject::KillObjectSub(GameObject* obj)
{
	if (!childList_.empty())
	{
		auto list = obj->GetChildList();
		auto it = list->begin();
		auto end = list->end();
		while (it != end)
		{
			KillObjectSub(*it);
			delete* it;
			it = list->erase(it);
		}
		list->clear();
	}
	obj->Release();
}