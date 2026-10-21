#pragma once
#include <DirectXMath.h>

//レイキャスト用構造体
struct RayCastData
{
    DirectX::XMFLOAT3 start;  // レイ発射位置
    DirectX::XMFLOAT3 dir;    // レイの向きベクトル
    float             dist;   // 衝突点までの距離
    bool              hit;    // レイが当たったか
    DirectX::XMFLOAT3 normal; // 法線

    RayCastData()
    {
        dist = INFINITY;
    }
};

class RayCast
{
public:
    void RayCasting(RayCastData* _data);

private:
    void IntersectRect();
};