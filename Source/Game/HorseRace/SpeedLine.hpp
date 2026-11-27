#pragma once
#include "GameObject.hpp"

class SpeedLine : public GameObject
{
public:
	SpeedLine(GameObject* parent);
	
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:
	int hImage_;
};