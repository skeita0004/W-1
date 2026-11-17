#include "Puddle.h"
#include "Engine/Model.h"

#include "Engine/Global.h"

#include <cmath>
#include <algorithm>

namespace
{
	const float MAX_SCALING_TIME{ 3.0f };
}

Puddle::Puddle( GameObject* _pParent ) : 
	GameObject(_pParent, "Puddle"),
	hModel_(-1),
	scalingTime_(MAX_SCALING_TIME)
{
}

void Puddle::Initialize()
{
	hModel_ = Model::Load( "Model/beverage.fbx" );
}

void Puddle::Update()
{
	scalingTime_ -= deltaTime / 1000.f;
	float t = std::clamp(1 - scalingTime_ / MAX_SCALING_TIME, 0.0f, 1.0f);
	float rate{ 1.0f - powf( 1.0f - t, 5.0f ) };

	transform_.scale_.x = std::lerp(1.0f, 3.0f, rate);
	transform_.scale_.z = std::lerp(1.0f, 3.0f, rate);
}

void Puddle::Draw()
{
	Model::SetTransform( hModel_, transform_ );
	Model::Draw(hModel_);
}

void Puddle::Release()
{
}
