#pragma once

#include "../Engine/GameObject.h"

class Table;

class Stage : public GameObject
{
public:
	Stage(GameObject *_pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

	inline static const uint32_t MAX_PLAYER_NUM{ 2 };

private:
	Table *pTable_;

};