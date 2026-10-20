#include "VirtualCameraOrtho.hpp"

VirtualCameraOrtho::VirtualCameraOrtho(const CameraType _type,
                                       const XMFLOAT3&  _upDir,
                                       const XMFLOAT2&  _viewSize,
                                       const float      _nearClip,
                                       const float      _farClip) :
                                       cameraType(_type),
                                       upDir_(_upDir),
                                       viewSize_(_viewSize),
                                       nearClip_(_nearClip),
                                       farClip_(_farClip)
{
}

void VirtualCameraOrtho::Init()
{
    matProjection_ = XMMatrixIdentity();
    matView_ = XMMatrixIdentity();

    transform_.position = XMFLOAT3(0, 0, 0);
    targetTransform_.position = XMFLOAT3(0, 1, 0);
}

void VirtualCameraOrtho::Update()
{
    matProjection_ = XMMatrixOrthographicLH(viewSize_.x,
                                            viewSize_.y,
                                            nearClip_,
                                            farClip_);

    XMVECTOR vTargetPos{XMLoadFloat3(&targetTransform_.position)};
    vTargetPos = XMVector3TransformCoord(vTargetPos, targetTransform_.GetCameraMatrix());
    XMStoreFloat3(&targetTransform_.position, vTargetPos);
    matView_ = XMMatrixLookAtLH(XMLoadFloat3(&transform_.position),
                                XMLoadFloat3(&targetTransform_.position),
                                XMLoadFloat3(&upDir_));
}

Transform& VirtualCameraOrtho::GetTransform()
{
    return transform_;
}

Transform& VirtualCameraOrtho::GetTargetTransform()
{
    return targetTransform_;
}

XMFLOAT2& VirtualCameraOrtho::GetViewSize()
{
    return viewSize_;
}

float& VirtualCameraOrtho::GetNearClip()
{
    return nearClip_;
}

float& VirtualCameraOrtho::GetFarClip()
{
    return farClip_;
}

const XMMATRIX& VirtualCameraOrtho::GetProjectionMat() const
{
    return matProjection_;
}

const XMMATRIX& VirtualCameraOrtho::GetViewMat() const
{
    return matView_;
}