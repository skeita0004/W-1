#include "WaterFlow.hpp"
#include "Model.hpp"

WaterFlow::WaterFlow(GameObject *_pParent) :
	GameObject(_pParent, "WaterFlow"),
	hModel_(-1)
{
}

void WaterFlow::Initialize()
{
	hModel_ = Model::Load("Model/ChickenRace/water_flow.fbx");
}

void WaterFlow::Update()
{
	transform_.rotate.x += 3;
}

void WaterFlow::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Renderer::Draw(hModel_);
}

void WaterFlow::Release()
{}
