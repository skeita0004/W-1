#pragma once
#include "Obstacle.hpp"

class FallingRock : public Obstacle
{
public:
	FallingRock(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Release() override;
};