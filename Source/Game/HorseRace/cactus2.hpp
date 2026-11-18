#pragma once
#include "Obstacle.hpp"

class cactus2 : public Obstacle
{
public:
	cactus2(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Release() override;
};