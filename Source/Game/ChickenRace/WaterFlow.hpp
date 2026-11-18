#pragma once

#include "GameObject.hpp"

class WaterFlow : public GameObject
{
public:
	WaterFlow(GameObject *_pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

private:
	int hModel_;
};