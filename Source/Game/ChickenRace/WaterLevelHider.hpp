#pragma once

#include "../Engine/GameObject.h"

class WaterLevelHider : public GameObject
{
public:
	WaterLevelHider(GameObject *_pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

private:
	int hImage_;
};