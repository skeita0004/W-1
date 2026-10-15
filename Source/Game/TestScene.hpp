#pragma once
#include "GameObject.hpp"

class SceneManager;

class TestScene : public GameObject
{
public:
	TestScene(GameObject* _pParent);

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:
    SceneManager* pSceneManager_;
};