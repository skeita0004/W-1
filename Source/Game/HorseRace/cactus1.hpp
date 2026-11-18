#pragma once
#include "Obstacle.hpp"

class cactus1 : public Obstacle
{
public:
	cactus1(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Release() override;
};
