#pragma once

#include "../Engine/GameObject.h"

#include <array>

class Player;

class Judge : public GameObject
{
public:
	Judge(GameObject *_pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

	void Judgement();

	inline static int confirmedCount{0};
	inline static std::string winnerName{"None"};
	inline static std::string reason{ "Don't know." };

	inline static int p1WaterLevel{};
	inline static int p2WaterLevel{};

	enum JudgeReason
	{
		NONE,

		SAME_WATER_LEVEL,
		WATER_OVERFLOW,
		MAX_REASON
	}; 

	inline static JudgeReason jr{NONE};

private:
	std::array<Player*, 2> pPlayers_;
};