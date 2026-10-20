#pragma once
#include <vector>
//#include <string>

#include <DirectXMath.h>

#include "IVirtualCamera.hpp"
#include "CameraType.hpp"

class Transform;

class CameraSet
{
public:
    CameraSet();

    void Init();
    void Release();

    /// @brief Currentとして設定されている
    /// カメラのTransformを返す
    /// @return カメラのTransform
    Transform& GetTransform();

    /// @brief 指定したカメラをCurrentにする
    /// @param _hCam カメラのハンドル
    void SetCurrent(int _hCam);

    /// @brief Currentとして設定されているカメラの
    /// 参照を返す
    /// @return VirtualCamera&
    IVirtualCamera* GetCurrent();

    IVirtualCamera* GetVirtualCamera(int _handle);

    // void FindCamera(CameraType _type);

    inline static int hNormalCamera{0};
    inline static int hShadowCamera{0};
    inline static int hReflectionCamera{0};

private:
    int Create(const CameraType _type,
               const XMFLOAT3&  _upDir,
               const float      _fovAngleDeg,
               const float      _nearClip,
               const float      _farClip);

    int CreateOrtho(const CameraType _type,
                    const XMFLOAT3&  _upDir,
                    const XMFLOAT2&  _viewSize,
                    const float      _nearClip,
                    const float      _farClip);

    inline static int                          hCurrent_{0};
    inline static std::vector<IVirtualCamera*> instances_{};
};