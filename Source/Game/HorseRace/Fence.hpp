#pragma once
#include "Obstacle.hpp"

class Fence : public Obstacle
{
public:
	Fence(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Release() override;

};
