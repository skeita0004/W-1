#include "Judge.h"

#include "Player.h"
#include "Stage.h"

Judge::Judge(GameObject *_pParent) :
	GameObject(_pParent, "Judge"),
	pPlayers_()
{
}

void Judge::Initialize()
{
	for (size_t i = 0; i < Stage::MAX_PLAYER_NUM; i++)
	{
		pPlayers_[i] = static_cast<Player*>(FindObject("Player" + std::to_string(i + 1)));
	}
}

void Judge::Update()
{
}

void Judge::Draw()
{
}

void Judge::Release()
{
	confirmedCount = 0;
}

void Judge::Judgement()
{
	jr = NONE;

	const int BEERMUG_CAPACITY_MILLILITER{ 1000 };
	p1WaterLevel = BEERMUG_CAPACITY_MILLILITER * pPlayers_[0]->GetWaterLevel();
	p2WaterLevel = BEERMUG_CAPACITY_MILLILITER * pPlayers_[1]->GetWaterLevel();

	bool p1OverFlow = pPlayers_[0]->IsOverFlow();
	bool p2OverFlow = pPlayers_[1]->IsOverFlow();

	if (p1OverFlow and p2OverFlow)
	{
		winnerName = "Draw";
		reason = { "Both were overflowed." };
		jr = WATER_OVERFLOW;
	}
	else if (p1OverFlow)
	{
		winnerName = pPlayers_[1]->GetObjectName();
		reason = { "Player1 was overflowed." };
		jr = WATER_OVERFLOW;
	}
	else if (p2OverFlow)
	{
		winnerName = pPlayers_[0]->GetObjectName();
		reason = { "Player2 was overflowed." };
		jr = WATER_OVERFLOW;
	}
	else
	{
		float p1WaterLevel = pPlayers_[0]->GetWaterLevel();
		float p2WaterLevel = pPlayers_[1]->GetWaterLevel();

		if (p1WaterLevel > p2WaterLevel)
		{
			winnerName = pPlayers_[0]->GetObjectName();
			reason = { "Player1's water level was greater of Player2." };
		}
		else if (fabs(p1WaterLevel - p2WaterLevel) < FLT_EPSILON) // draw
		{
			winnerName = "Draw";
			reason = { "Both are same water level." };
			jr = SAME_WATER_LEVEL;
		}
		else
		{
			winnerName = pPlayers_[1]->GetObjectName();
			reason = { "Player2's water level was greater of Player1." };
		}
	}
}
