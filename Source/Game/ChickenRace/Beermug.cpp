#include "Beermug.hpp"

#include "Model.hpp"

#include "WaterLevelHider.hpp"
#include "Beverage.hpp"
#include "Bin.hpp"
#include "Judge.hpp"
#include "Puddle.hpp"

#include <imgui.h>

namespace
{
	const float MAX_WATER_LEVEL{ 137.f };
	const float OVERFLOW_RATE{ 1.025f };
}

Beermug::Beermug(GameObject *_pParent) :
	GameObject(_pParent, "Beermug"),
	hModels_(),
	beerMugID_(0),
	waterLevel_(0),
	pWLH_(),
	pBeverage_(),
	pPuddle_(),
	isFilling_(false),
	isWLHInstanced_(false),
	isOverFlow_(false)
{
}

void Beermug::Initialize()
{
	pBeverage_ = Instantiate<Beverage>(this);
	beerMugID_ = beerMugCount_ + 1;

	hModels_[BEERMUG_CROSS_SECTION_L] = Model::Load("Model/ChickenRace/beermug_L.fbx");
	hModels_[BEERMUG_CROSS_SECTION_R] = Model::Load("Model/ChickenRace/beermug_R.fbx");
	hModels_[BEERMUG]                 = Model::Load("Model/ChickenRace/wood_beermug_mock.fbx");

	beerMugCount_ += 1;
}

void Beermug::Update()
{
	// 注ぎ中
	if (waterLevel_ <= OVERFLOW_RATE)
	{
		if (isFilling_)
		{
			waterLevel_ += 0.0025;
			pBeverage_->SetScale(0.99f, MAX_WATER_LEVEL * waterLevel_, 0.99f);
		}
	}

	// 半分超えた
	if (waterLevel_ >= 0.5f and isWLHInstanced_ == false)
	{
		if (pWLH_ == nullptr)
		{
			pWLH_ = Instantiate<WaterLevelHider>(this);
		}
		pWLH_->SetPosition( 0.125f, 1.6f, -1.0f );
		if (beerMugID_ == 2)
		{
			pWLH_->SetPosition( -0.125f, 1.6f, -1.0f );
		}
		isWLHInstanced_ = true;
	}

	// 溢れた
	if (waterLevel_ >= OVERFLOW_RATE)
	{
		if (pPuddle_ == nullptr)
		{
			pPuddle_ = Instantiate<Puddle>( this );
		}

		isOverFlow_ = true;
		modelIdx_ = 2;
     	if (beerMugID_ == 2) 
		{
			transform_.rotate.y = 180.f;
		}

		if (pWLH_)
		{
			if (not(pWLH_->IsDead()))
			{
				pWLH_->KillMe();
			}
		}
	}

	if (Judge::confirmedCount == 2)
	{
		if (pWLH_)
		{
			if (not(pWLH_->IsDead()))
			{
				pWLH_->KillMe();
			}
		}
	}

	isFilling_ = false;
}

void Beermug::Draw()
{
	Model::SetTransform(hModels_[modelIdx_], transform_);
	Model::Draw(hModels_[modelIdx_]);
}

void Beermug::Release()
{
	beerMugCount_ = 0;
}

void Beermug::Fill()
{
	isFilling_ = true;
}
