#pragma once

#include "GameObject.hpp"

class TestScene : public GameObject
{
public:
	TestScene(GameObject* _pParent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:
	int hModel_;
};