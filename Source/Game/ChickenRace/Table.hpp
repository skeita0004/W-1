#pragma once

#include "GameObject.hpp"
#include "Stage.hpp"

#include <array>

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