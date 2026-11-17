#include "WaterFlow.h"
#include "Engine/Model.h"

WaterFlow::WaterFlow(GameObject *_pParent) :
	GameObject(_pParent, "WaterFlow"),
	hModel_(-1)
{
}

void WaterFlow::Initialize()
{
	hModel_ = Model::Load("model/water_flow.fbx");
}

void WaterFlow::Update()
{
	transform_.rotate_.x += 3;
}

void WaterFlow::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void WaterFlow::Release()
{}
