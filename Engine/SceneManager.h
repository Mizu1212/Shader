#pragma once
#include "GameObject.h"

//�Q�[���ɓo�ꂷ��V�[��
enum SCENE_ID
{
	SCENE_ID_PlayScene = 0,
	SCENE_ID_GameOver,
	SCENE_ID_GameStart,
};

//�V�[���؂�ւ���S������I�u�W�F�N�g
class SceneManager : public GameObject
{
public:
	//�R���X�g���N�^
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�V�[���؂�ւ��i���ۂɐ؂�ւ��̂͂��̎��̃t���[���j
	void ChangeScene(SCENE_ID next);

private:
	SCENE_ID firstSceneID_;	//���݂̃V�[��
	SCENE_ID nextSceneID_;		//���̃V�[��

};