#pragma once

#include "../Engine/GameObject.h"

class TitleLogo : public GameObject
{
public:
	TitleLogo(GameObject* _pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

private:
	int hImage_;
};