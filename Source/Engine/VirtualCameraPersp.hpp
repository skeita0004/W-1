#pragma once
#include "Transform.hpp"
#include "IVirtualCamera.hpp"

class VirtualCameraPersp final : public IVirtualCamera
{
public:
    VirtualCameraPersp(const XMFLOAT3&  _upDir,
                       const float      _fovAngleDeg,
                       const float      _nearClip,
                       const float      _farClip);

    void Init()   override;
    void Update() override;

    Transform      &GetTransform()             override;
    Transform      &GetTargetTransform()       override;
    float          &GetFovAngleDeg();
    float          &GetNearClip()              override;
    float          &GetFarClip()               override;
    const XMMATRIX &GetProjectionMat()   const override;
    const XMMATRIX &GetViewMat()         const override;
    const XMMATRIX &GetBillboardMat()    const;

private:
    Transform transform_;
    Transform targetTransform_;
    XMFLOAT3  upDir_;
    float     fovAngleDeg_;
    float     nearClip_;
    float     farClip_;
    XMMATRIX  matProjection_;
    XMMATRIX  matView_;
    XMMATRIX  matBillboard_;

    inline static const float DEFAULT_NEAR_CLIP{ 0.1f };
    inline static const float DEFAULT_FAR_CLIP{1000.f};
};