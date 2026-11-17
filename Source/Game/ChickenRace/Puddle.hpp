#pragma once

#include "../Engine/GameObject.h"

class Puddle : public GameObject
{
public:
	Puddle( GameObject* _pParent );

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

private:
	int hModel_;
	float scalingTime_;
};