#pragma once
#include "GameObject.hpp"
#include "EffekseerVFX.h"

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
	int hModel_;
	std::shared_ptr<EffekseerLib::EFKTransform> effectTsfrm;
};