#pragma once
#include "Engine/GameObject.h"


//���������Ǘ�����N���X
class dounatu : public GameObject
{
    int hModel_;    //���f���ԍ�
    GameObject* pBallet;
public:
    //�R���X�g���N�^
    dounatu(GameObject* parent);

    //�f�X�g���N�^
    ~dounatu();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};
