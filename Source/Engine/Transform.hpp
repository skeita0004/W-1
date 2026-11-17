#pragma once
#include <DirectXMath.h>

using namespace DirectX;

/// @brief 座標、座標変換を管理するクラス
class Transform
{
public:
    XMFLOAT3   position; //位置
    XMFLOAT3   rotate;   //向き
    XMFLOAT3   scale;    //拡大率

    Transform* pParent; // 親

    Transform();

    ~Transform();

    //各行列の計算
    void Calculation();

    //ワールド行列を取得
    const XMMATRIX GetWorldMatrix();
    const XMMATRIX GetNormalMatrix();
    const XMMATRIX GetCameraMatrix();

    const XMMATRIX GetTranslateMatrix();
    const XMMATRIX GetRotateMatrix();
    const XMMATRIX GetScaleMatrix();

    static inline XMFLOAT3 Float3Add(const XMFLOAT3& _lhs, const XMFLOAT3& _rhs)
    {
        return
        {
            _lhs.x + _rhs.x,
            _lhs.y + _rhs.y,
            _lhs.z + _rhs.z
        };
    }

private:
    static inline XMMATRIX matTranslate{ XMMatrixIdentity() };
    static inline XMMATRIX matRotate{ XMMatrixIdentity() };
    static inline XMMATRIX matScale{ XMMatrixIdentity() };
};

