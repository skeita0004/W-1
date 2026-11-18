#pragma once
#include "Obstacle.hpp"

class Rock : public Obstacle
{
public:
	Rock(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Release() override;
};