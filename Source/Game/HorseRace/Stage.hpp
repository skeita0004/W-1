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

	int widthCount_ = 10;
	int lengthCount_ = 200;

	float tileSize_ = 200.0f;

	std::vector<XMFLOAT3> floors;

	Horse* horse;

	std::vector<Obstacle*> obs;

	void LoadStageCsv();
};