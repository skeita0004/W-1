#pragma once

#include "../Engine/GameObject.h"

class Beermug;
class WaterFlowOrigin;

class Bin : public GameObject
{
public:
	Bin(GameObject *_pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

	bool IsPouring()
	{
		return isPouring_;
	}
	
	bool IsConfirmed()
	{
		return isConfirmed_;
	}

	// 注ぐ
	void Pouring();

	// 液量確定
	void Conform();

private:
	inline static int binCount_{ 0 };
	int binID_;

	int  hModel_;

	// 注ぎ操作を行った時間（フレーム単位）
	int  pouringTime_;
	bool isPouring_;
	bool isConfirmed_;

	Beermug* pBeermug_;

	WaterFlowOrigin *pWFO_;

	// Beermugのインスタンスを取得できたか？
	bool  beermugInstanceRetrieved_;

};