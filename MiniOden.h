#pragma once
#include "Engine/GameObject.h"
class MiniOden :
    public GameObject
{
    int hModel_;
public:
    //�R���X�g���N�^
    MiniOden(GameObject* parent);

    //�f�X�g���N�^
    ~MiniOden();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};

