#pragma once
#include "Engine/GameObject.h"

//地面を管理するクラス
class Ground : public GameObject
{

	int hModel_;    //モデル番号


public:
	//コンストラクタ
	Ground(GameObject* parent);

	//デストラクタ
	~Ground();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;


	//モデル番号のゲッター
	int GetModelHandle() { return hModel_; }
	//int hBuilding_[1];              //建物モデル番号
	//Transform buildingTransform_[1]; //建物のTransform
};