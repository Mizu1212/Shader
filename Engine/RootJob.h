#pragma once
#include "GameObject.h"
class RootJob :
    public GameObject
{
public:
	//�R���X�g���N�^
	//�����@GameObject�^�̃|�C���^
	RootJob(GameObject* parent);
	~RootJob();
	 void Initialize();
	 void Update();
	 void Draw();
	 void Release();
};

