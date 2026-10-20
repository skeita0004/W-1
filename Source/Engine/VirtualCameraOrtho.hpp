#pragma once

#include "IVirtualCamera.hpp"
#include "CameraType.hpp"

class VirtualCameraOrtho final : public IVirtualCamera
{
public:
    VirtualCameraOrtho(const CameraType _type,
                       const XMFLOAT3&  _upDir,
                       const XMFLOAT2&  _viewSize,
                       const float      _nearClip,
                       const float      _farClip);

    void Init()   override;
    void Update() override;

    Transform&      GetTransform()       override;
    Transform&      GetTargetTransform() override;
    XMFLOAT2&       GetViewSize();
    float&          GetNearClip()        override;
    float&          GetFarClip()         override;
    const XMMATRIX& GetProjectionMat()   const override;
    const XMMATRIX& GetViewMat()         const override;

    CameraType cameraType;

private:
    Transform transform_;
    Transform targetTransform_;
    XMFLOAT3  upDir_;
    XMFLOAT2  viewSize_;
    float     nearClip_;
    float     farClip_;
    XMMATRIX  matProjection_;
    XMMATRIX  matView_;

    inline static const float DEFAULT_NEAR_CLIP{0.1f};
    inline static const float DEFAULT_FAR_CLIP{1000.f};
};