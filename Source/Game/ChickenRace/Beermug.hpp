#pragma once

#include "GameObject.hpp"

class WaterLevelHider;
class Beverage;
class Puddle;

class Beermug : public GameObject
{
public:
	Beermug(GameObject *_pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

	void SetModelIndex(int _index)
	{
		if (_index > 2)
		{
			return;
		}
		modelIdx_ = _index;
	}

	float GetWaterLevel()
	{
		return waterLevel_;
	}

	bool IsOverFlow()
	{
		return isOverFlow_;
	}

	enum BeermugVariation
	{
		BEERMUG_CROSS_SECTION_L = 0,
		BEERMUG_CROSS_SECTION_R,
		BEERMUG,
	};

	// 液量を増加させる
	void Fill();

	inline static const float wlhSpawnLevel{ 0.5f };
private:

	inline static int beerMugCount_{0};
	int beerMugID_;

	int hModels_[3];
	int modelIdx_ = 0;

	WaterLevelHider *pWLH_;
	Beverage *pBeverage_;
	Puddle* pPuddle_;

	float waterLevel_;
	bool  isFilling_;
	bool  isWLHInstanced_;
	bool  isOverFlow_;
};