#pragma once
#include "../../Engine/GameObject.hpp"

class Horse : public GameObject
{
public:
	Horse(GameObject* parent);
	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};