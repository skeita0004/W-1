#pragma once

#pragma once

#include "GameObject.hpp"

class Beverage : public GameObject
{
public:
	Beverage(GameObject *_pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

private:
	int hModel_;
};