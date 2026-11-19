#pragma once
#include "Engine/GameObject.h"

//地面を管理するクラス
class Infomation : public GameObject
{
	int hPict_;    //画像番号

public:
	//コンストラクタ
	Infomation(GameObject* parent);

	//デストラクタ
	~Infomation();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};