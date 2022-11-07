#pragma once
#include "Engine/GameObject.h"
#include "Engine/SphereCollider.h"

//���������Ǘ�����N���X
class TestModel : public GameObject
{
    int hModel_;    //���f���ԍ�
    
public:
    //�R���X�g���N�^
    TestModel(GameObject* parent);

    //�f�X�g���N�^
    ~TestModel();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    //�����ɓ�������
    //�����FpTarget ������������
    void OnCollision(GameObject* pTarget) override;
};