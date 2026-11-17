#pragma once

#include "../Engine/GameObject.h"
#include <array>
#include "Stage.h"

class Beermug;
class Player;

class Table : public GameObject
{
public:
	Table(GameObject *_pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

private:
	int hModel_;

	std::array<Player*, Stage::MAX_PLAYER_NUM> pPlayers_;
};