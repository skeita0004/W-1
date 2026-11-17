#pragma once

#include "../Engine/GameObject.h"

class WaterFlow;

class WaterFlowOrigin : public GameObject
{
public:
	WaterFlowOrigin(GameObject *_pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;
	
	void WFVisible(bool _wfVisible)
	{
		wfVisible_ = _wfVisible;
	}

private:
	int hModel_;
	bool wfVisible_;
	WaterFlow *pWaterFlow_;
};