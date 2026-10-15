#pragma once

#include "Transform.hpp"

class IVirtualCamera
{
public:
    virtual ~IVirtualCamera() = default;

    virtual void Init()   = 0;
    virtual void Update() = 0;

    virtual Transform      &GetTransform()           = 0;
    virtual Transform      &GetTargetTransform()     = 0;
    virtual float          &GetFarClip()             = 0;
    virtual float          &GetNearClip()            = 0;
    virtual const XMMATRIX &GetProjectionMat() const = 0;
    virtual const XMMATRIX &GetViewMat()       const = 0;
};