#include "WaterFlowOrigin.h"
#include "WaterFlow.h"

#include "Engine/Model.h"

WaterFlowOrigin::WaterFlowOrigin(GameObject *_pParent) :
	GameObject(_pParent, "WaterFlowOrigin"),
	hModel_(-1)
{
}

void WaterFlowOrigin::Initialize()
{
	hModel_ = Model::Load("model/water_flow_origin.fbx");

	pWaterFlow_ = Instantiate<WaterFlow>(this);

	pWaterFlow_->Invisible();

	XMFLOAT3 wfPos{Model::GetBonePosition(hModel_, "WaterFlowTail")};
	pWaterFlow_->SetPosition(wfPos);
}

void WaterFlowOrigin::Update()
{
	if (wfVisible_)
	{
		pWaterFlow_->Visible();
	}
}

void WaterFlowOrigin::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void WaterFlowOrigin::Release()
{
}
