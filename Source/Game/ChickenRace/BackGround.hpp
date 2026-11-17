#pragma once

#include "GameObject.hpp"

class BackGround : public GameObject
{
public:
	BackGround(GameObject* _pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

private:
	int hImage_;
};