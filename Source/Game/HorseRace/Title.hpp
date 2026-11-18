#pragma once
#include "../Engine/GameObject.h"

class Title : public GameObject
{
public:
	Title(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	int hImage_;
};