#pragma once
#include "GameObject.hpp"
#include "Horse.hpp"

class ResultScene : public GameObject
{
public:
	ResultScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	int hImage_;
	Horse* horse;
};