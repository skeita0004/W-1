#pragma once
#include <vector>

#include <DirectXMath.h>

#include "IVirtualCamera.hpp"

class Transform;

class CameraSet
{
public:
    CameraSet();

    void Init();
    void Release();

    /// @brief カメラを作成する
    /// @note  作成してすぐにCurrentとして
    /// 登録されるわけではない。
    /// @return 作成されたカメラのハンドル
    int Create(const DirectX::XMFLOAT3 &_upDir,
               const float              _fovAngleDeg,
               const float              _nearClip,
               const float              _farClip);

    
    int CreateOrtho(const DirectX::XMFLOAT3 &_upDir,
                    const XMFLOAT2           _viewSize,
                    const float              _nearClip,
                    const float              _farClip);

    /// @brief Currentとして設定されている
    /// カメラのTransformを返す
    /// @return カメラのTransform
    Transform &GetTransform();

    /// @brief 指定したカメラをCurrentにする
    /// @param _hCam カメラのハンドル
    void SetCurrent(int _hCam);

    /// @brief Currentとして設定されているカメラの
    /// 参照を返す
    /// @return VirtualCamera&
    IVirtualCamera *GetCurrent();

private:
    inline static int hCurrent_{0};
    inline static std::vector<IVirtualCamera*> instances_{};
};