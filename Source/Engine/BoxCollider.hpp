#pragma once
#include "Collider.hpp"


//-----------------------------------------------------------
//箱型の当たり判定
//-----------------------------------------------------------
class BoxCollider :	public Collider
{
	//Colliderクラスのprivateメンバにアクセスできるようにする
	friend class Collider;

public:
	//コンストラクタ（当たり判定の作成）
	//引数：basePos	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
	//引数：size	当たり判定のサイズ（幅、高さ、奥行き）
	BoxCollider(XMFLOAT3 basePos, XMFLOAT3 size);

private:
	//接触判定
	//引数：target	相手の当たり判定
	//戻値：接触してればtrue
	bool IsHit(Collider* target) override;
};

