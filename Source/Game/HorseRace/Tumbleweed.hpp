#pragma once
#include "Obstacle.hpp"

class Tumbleweed : public Obstacle
{
public:
	Tumbleweed(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Release() override;
};