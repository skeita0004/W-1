#pragma once
#include "GameObject.hpp"
#include "Horse.hpp"
#include "Obstacle.hpp"

class Stage : public GameObject
{
public:
	Stage(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	int hModel_;

	Horse* horse;

	std::vector<Obstacle*> obs;

	void LoadStageCsv();
};