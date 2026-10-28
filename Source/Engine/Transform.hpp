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

    XMFLOAT3 GetPosition()
    {
        return position;
    }
    XMFLOAT3 GetRotate()
    {
        return rotate;
    }
    XMFLOAT3 GetScale()
    {
        return scale;
    }
    XMFLOAT3 GetWorldPosition()
    {
        return Transform::Float3Add(pParent->position, position);
    }
    XMFLOAT3 GetWorldRotate()
    {
        return Transform::Float3Add(pParent->rotate, rotate);
    }
    XMFLOAT3 GetWorldScale()
    {
        return Transform::Float3Add(pParent->scale, scale);
    }

    void SetPosition(XMFLOAT3 _position)
    {
        position = _position;
    }
    void SetPosition(float x, float y, float z)
    {
        SetPosition(XMFLOAT3(x, y, z));
    }
    void SetRotate(XMFLOAT3 _rotate)
    {
        rotate = _rotate;
    }
    void SetRotate(float x, float y, float z)
    {
        SetRotate(XMFLOAT3(x, y, z));
    }
    void SetRotateX(float x)
    {
        SetRotate(x, rotate.y, rotate.z);
    }
    void SetRotateY(float y)
    {
        SetRotate(rotate.x, y, rotate.z);
    }
    void SetRotateZ(float z)
    {
        SetRotate(rotate.x, rotate.y, z);
    }
    void SetScale(XMFLOAT3 scale)
    {
        scale = scale;
    }
    void SetScale(float _scale)
    {
        SetScale(XMFLOAT3(_scale, _scale, _scale));
    }
    void SetScale(float x, float y, float z)
    {
        SetScale(XMFLOAT3(x, y, z));
    }

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

