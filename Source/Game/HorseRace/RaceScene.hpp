#pragma once
#include "GameObject.hpp"
#include "Horse.hpp"

class SpeedLine;

class RaceScene : public GameObject
{
public:
	RaceScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	int hImage_;
	int hImageS_;
	int hBgm_;

	Horse* horse;
	SpeedLine* speedLine_;
};