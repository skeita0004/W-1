#include "WaterFlowOrigin.hpp"
#include "WaterFlow.hpp"

#include "Model.hpp"

WaterFlowOrigin::WaterFlowOrigin(GameObject *_pParent) :
	GameObject(_pParent, "WaterFlowOrigin"),
	hModel_(-1)
{
}

void WaterFlowOrigin::Initialize()
{
	hModel_ = Model::Load("Model/ChickenRace/water_flow_origin.fbx");

	pWaterFlow_ = Instantiate<WaterFlow>(this);

	pWaterFlow_->DisableDraw();

	XMFLOAT3 wfPos{Model::GetBonePosition(hModel_, "WaterFlowTail")};
	pWaterFlow_->SetPosition(wfPos);
}

void WaterFlowOrigin::Update()
{
	if (wfVisible_)
	{
		pWaterFlow_->EnableDraw();
	}
}

void WaterFlowOrigin::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Renderer::Draw(hModel_);
}

void WaterFlowOrigin::Release()
{
}
